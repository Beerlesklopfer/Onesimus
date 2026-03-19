//! Unified command representation (CommandAST).

/// Source protocol of the command.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum CommandSource {
    Cli,
    JsonRpc,
    Grpc,
    Internal,
}

/// Response format for command output.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ResponseFormat {
    Default,
    Text,
    Api,
    Json,
    Grpc,
}

/// A single command argument.
#[derive(Debug, Clone)]
pub struct Argument {
    pub key: String,
    pub value: Option<String>,
}

/// Parsed command ready for execution.
#[derive(Debug, Clone)]
pub struct CommandAst {
    pub command_name: String,
    pub arguments: Vec<Argument>,
    pub session_id: String,
    pub source: CommandSource,
    pub response_format: ResponseFormat,
}
