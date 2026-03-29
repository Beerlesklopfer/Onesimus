//! Send a command to SD and read the response.
//!
//! Connects via TLS-PSK, authenticates, sends command, reads until EOD.

use bareos_bnet::{BnetMessage, BnetSignal};

/// Connect to SD, send a single command, read response, disconnect.
pub fn sd_command(
    address: &str,
    port: u16,
    password: &str,
    director_name: &str,
    command: &str,
) -> Result<String, String> {
    let mut sd = crate::connect::connect_sd(address, port, password, director_name)?;

    sd.send_str(command).map_err(|e| format!("send: {e}"))?;

    let mut response = String::new();
    loop {
        match sd.recv() {
            Ok(BnetMessage::Data(data)) => {
                response.push_str(&String::from_utf8_lossy(&data));
            }
            Ok(BnetMessage::Signal(BnetSignal::Eod | BnetSignal::Terminate)) => break,
            Ok(BnetMessage::Signal(_)) => continue,
            Err(_) => break,
        }
    }
    let _ = sd.send_signal(BnetSignal::Terminate);
    Ok(response)
}
