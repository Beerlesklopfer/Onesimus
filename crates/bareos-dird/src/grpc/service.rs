//! DirectorService implementation for tonic gRPC.

use std::pin::Pin;
use std::sync::Arc;

use tokio::sync::mpsc;
use tokio_stream::wrappers::ReceiverStream;
use tonic::{Request, Response, Status, Streaming};

use bareos_proto_rs::bareos::director::{
    director_server::Director,
    CommandRequest, CommandResponse,
    ListCommandsRequest, ListCommandsResponse, CommandInfo,
    SessionMessage, session_message,
};

use crate::command_ast::{CommandAst, CommandSource, ResponseFormat, Argument};
use crate::command_queue::{CommandQueue, CommandResult};
use crate::message_bus::{BusMessage, MessageBus};
use crate::session_manager::SessionManager;

pub struct DirectorServiceImpl {
    session_mgr: Arc<SessionManager>,
    bus: Arc<MessageBus>,
    cmd_queue: Arc<CommandQueue>,
}

impl DirectorServiceImpl {
    pub fn new(
        session_mgr: Arc<SessionManager>,
        bus: Arc<MessageBus>,
        cmd_queue: Arc<CommandQueue>,
    ) -> Self {
        Self { session_mgr, bus, cmd_queue }
    }

    fn command_request_to_ast(&self, req: &CommandRequest) -> CommandAst {
        CommandAst {
            command_name: req.command_name.clone(),
            arguments: req.arguments.iter().map(|a| {
                let value = match &a.value {
                    Some(bareos_proto_rs::bareos::director::command_argument::Value::StringValue(s)) => {
                        Some(s.clone())
                    }
                    Some(bareos_proto_rs::bareos::director::command_argument::Value::JsonValue(b)) => {
                        Some(String::from_utf8_lossy(b).into_owned())
                    }
                    None => None,
                };
                Argument { key: a.key.clone(), value }
            }).collect(),
            session_id: req.session_id.clone(),
            source: CommandSource::Grpc,
            response_format: ResponseFormat::Grpc,
        }
    }
}

#[tonic::async_trait]
impl Director for DirectorServiceImpl {
    async fn execute_command(
        &self,
        request: Request<CommandRequest>,
    ) -> Result<Response<CommandResponse>, Status> {
        let req = request.into_inner();
        let ast = self.command_request_to_ast(&req);

        // Submit to command queue
        let result_rx = self.cmd_queue.submit(ast, 0)
            .ok_or_else(|| Status::resource_exhausted("command queue full"))?;

        // Wait for result
        let result = result_rx.recv()
            .map_err(|_| Status::internal("command execution failed"))?;

        Ok(Response::new(CommandResponse {
            success: result.success,
            text_output: result.text_output,
            json_output: Vec::new(),
            error_message: result.error_message,
        }))
    }

    async fn list_commands(
        &self,
        _request: Request<ListCommandsRequest>,
    ) -> Result<Response<ListCommandsResponse>, Status> {
        // Return the 88 known commands
        let commands = KNOWN_COMMANDS.iter().map(|(name, help, usage)| {
            CommandInfo {
                name: name.to_string(),
                help: help.to_string(),
                usage: usage.to_string(),
                allowed_in_runscript: false,
                audit_event: false,
            }
        }).collect();

        Ok(Response::new(ListCommandsResponse { commands }))
    }

    type InteractiveSessionStream = ReceiverStream<Result<SessionMessage, Status>>;

    async fn interactive_session(
        &self,
        request: Request<Streaming<SessionMessage>>,
    ) -> Result<Response<Self::InteractiveSessionStream>, Status> {
        let mut stream = request.into_inner();
        let (tx, rx) = mpsc::channel(32);

        let bus = self.bus.clone();
        let cmd_queue = self.cmd_queue.clone();

        tokio::spawn(async move {
            // Subscribe to bus for push events
            let bus_rx = bus.subscribe();

            loop {
                tokio::select! {
                    // Read from client
                    msg = stream.message() => {
                        match msg {
                            Ok(Some(session_msg)) => {
                                if session_msg.r#type() == session_message::Type::Command {
                                    // Execute command
                                    if let Some(cmd_req) = session_msg.command {
                                        let ast = CommandAst {
                                            command_name: cmd_req.command_name,
                                            arguments: Vec::new(),
                                            session_id: session_msg.session_id.clone(),
                                            source: CommandSource::Grpc,
                                            response_format: ResponseFormat::Grpc,
                                        };

                                        if let Some(result_rx) = cmd_queue.submit(ast, 0) {
                                            if let Ok(result) = result_rx.recv() {
                                                let response = SessionMessage {
                                                    r#type: session_message::Type::Response as i32,
                                                    session_id: session_msg.session_id,
                                                    response: Some(CommandResponse {
                                                        success: result.success,
                                                        text_output: result.text_output,
                                                        json_output: Vec::new(),
                                                        error_message: result.error_message,
                                                    }),
                                                    ..Default::default()
                                                };
                                                if tx.send(Ok(response)).await.is_err() {
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                            Ok(None) => break, // Stream ended
                            Err(_) => break,    // Error
                        }
                    }

                    // Check for bus messages (non-blocking)
                    _ = tokio::time::sleep(tokio::time::Duration::from_millis(100)) => {
                        while let Ok(bus_msg) = bus_rx.try_recv() {
                            let push_event = super::push_events::to_push_event(&bus_msg);
                            let session_msg = SessionMessage {
                                r#type: session_message::Type::PushEvent as i32,
                                push_event: Some(push_event),
                                ..Default::default()
                            };
                            if tx.send(Ok(session_msg)).await.is_err() {
                                return;
                            }
                        }
                    }
                }
            }
        });

        Ok(Response::new(ReceiverStream::new(rx)))
    }
}

/// Well-known Director commands (subset for ListCommands).
static KNOWN_COMMANDS: &[(&str, &str, &str)] = &[
    ("status", "Report status", "status [dir | client | storage | all]"),
    ("version", "Print Director version", "version"),
    ("help", "Print help on command", "help [command]"),
    (".help", "Print machine-readable help", ".help [command]"),
    ("run", "Run a job", "run [job=name] [client=name] [level=type]"),
    ("cancel", "Cancel a job", "cancel [jobid=id]"),
    ("list", "List catalog contents", "list [jobs | volumes | pools | clients]"),
    ("llist", "Long list catalog contents", "llist [jobs | volumes]"),
    ("restore", "Restore files", "restore [where=path]"),
    ("quit", "Terminate bconsole", "quit"),
    ("reload", "Reload configuration", "reload"),
    ("enable", "Enable resource", "enable [client | job | schedule | storage]"),
    ("disable", "Disable resource", "disable [client | job | schedule | storage]"),
    ("label", "Label a volume", "label [volume=name] [storage=name]"),
    ("mount", "Mount storage device", "mount [storage=name]"),
    ("unmount", "Unmount storage device", "unmount [storage=name]"),
    ("update", "Update catalog records", "update [pool | volume | slots]"),
    ("configure", "Configure resources", "configure [add | delete | update]"),
    (".clients", "List clients", ".clients"),
    (".jobs", "List jobs", ".jobs"),
    (".pools", "List pools", ".pools"),
    (".storage", "List storage", ".storage"),
    (".filesets", "List filesets", ".filesets"),
    (".defaults", "List defaults", ".defaults [job=name]"),
    (".jobstatus", "List job status codes", ".jobstatus"),
    (".sql", "Execute SQL", ".sql query=\"...\""),
    ("time", "Print current time", "time"),
    ("estimate", "Estimate backup size", "estimate [job=name] [listing]"),
    ("prune", "Prune catalog records", "prune [files | jobs | volumes]"),
    ("purge", "Purge catalog records", "purge [files | jobs | volumes]"),
    ("truncate", "Truncate purged volumes", "truncate [volume=name]"),
];
