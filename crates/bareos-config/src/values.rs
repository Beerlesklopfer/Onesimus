//! Config value parsing: booleans, times, sizes, strings.

use std::time::Duration;

/// Parse a boolean value: yes/no/true/false/on/off/1/0
pub fn parse_bool(input: &str) -> Result<((), bool), String> {
    match input.trim().to_lowercase().as_str() {
        "yes" | "true" | "on" | "1" => Ok(((), true)),
        "no" | "false" | "off" | "0" => Ok(((), false)),
        other => Err(format!("invalid boolean: {other}")),
    }
}

/// Parse a time duration with units.
/// Examples: "30", "30 seconds", "5 minutes", "2 hours", "365 days"
pub fn parse_time(input: &str) -> Result<((), Duration), String> {
    let input = input.trim();

    // Split into number and optional unit
    let (num_str, unit_str) = split_number_unit(input);
    let number: u64 = num_str.parse().map_err(|e| format!("invalid number: {e}"))?;

    let multiplier = match unit_str.to_lowercase().as_str() {
        "" | "s" | "sec" | "secs" | "second" | "seconds" => 1u64,
        "min" | "mins" | "minute" | "minutes" => 60,
        "h" | "hr" | "hrs" | "hour" | "hours" => 3600,
        "day" | "days" => 86400,
        "week" | "weeks" => 604800,
        "month" | "months" => 2592000,
        "year" | "years" => 31536000,
        other => return Err(format!("unknown time unit: {other}")),
    };

    Ok(((), Duration::from_secs(number * multiplier)))
}

/// Parse a size with units (returns bytes).
/// Examples: "50G", "1024K", "100 MB", "1 GiB"
pub fn parse_size(input: &str) -> Result<((), u64), String> {
    let input = input.trim();
    let (num_str, unit_str) = split_number_unit(input);
    let number: u64 = num_str.parse().map_err(|e| format!("invalid number: {e}"))?;

    let multiplier = match unit_str.to_lowercase().as_str() {
        "" | "b" => 1u64,
        // Binary
        "kib" | "ki" => 1024,
        "mib" | "mi" => 1024 * 1024,
        "gib" | "gi" => 1024 * 1024 * 1024,
        "tib" | "ti" => 1024u64 * 1024 * 1024 * 1024,
        // Decimal
        "k" | "kb" => 1000,
        "m" | "mb" => 1_000_000,
        "g" | "gb" => 1_000_000_000,
        "t" | "tb" => 1_000_000_000_000,
        other => return Err(format!("unknown size unit: {other}")),
    };

    Ok(((), number * multiplier))
}

/// Split "365 days" or "50G" into ("365", "days") or ("50", "G").
fn split_number_unit(input: &str) -> (&str, &str) {
    let input = input.trim();
    // Find where digits end
    let num_end = input
        .find(|c: char| !c.is_ascii_digit())
        .unwrap_or(input.len());
    let num_str = &input[..num_end];
    let unit_str = input[num_end..].trim();
    (num_str, unit_str)
}

/// Parse a quoted string value, stripping outer quotes.
pub fn parse_quoted_string(input: &str) -> Option<String> {
    let input = input.trim();
    if input.starts_with('"') && input.ends_with('"') && input.len() >= 2 {
        let inner = &input[1..input.len() - 1];
        // Handle escape sequences
        let mut result = String::with_capacity(inner.len());
        let mut chars = inner.chars();
        while let Some(c) = chars.next() {
            if c == '\\' {
                if let Some(next) = chars.next() {
                    result.push(next);
                }
            } else {
                result.push(c);
            }
        }
        Some(result)
    } else {
        None
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn bool_values() {
        assert_eq!(parse_bool("yes").unwrap().1, true);
        assert_eq!(parse_bool("Yes").unwrap().1, true);
        assert_eq!(parse_bool("YES").unwrap().1, true);
        assert_eq!(parse_bool("true").unwrap().1, true);
        assert_eq!(parse_bool("on").unwrap().1, true);
        assert_eq!(parse_bool("1").unwrap().1, true);
        assert_eq!(parse_bool("no").unwrap().1, false);
        assert_eq!(parse_bool("No").unwrap().1, false);
        assert_eq!(parse_bool("false").unwrap().1, false);
        assert_eq!(parse_bool("off").unwrap().1, false);
        assert_eq!(parse_bool("0").unwrap().1, false);
    }

    #[test]
    fn time_values() {
        assert_eq!(parse_time("30").unwrap().1, Duration::from_secs(30));
        assert_eq!(parse_time("30 seconds").unwrap().1, Duration::from_secs(30));
        assert_eq!(parse_time("5 minutes").unwrap().1, Duration::from_secs(300));
        assert_eq!(parse_time("2 hours").unwrap().1, Duration::from_secs(7200));
        assert_eq!(parse_time("1 day").unwrap().1, Duration::from_secs(86400));
        assert_eq!(parse_time("365 days").unwrap().1, Duration::from_secs(31536000));
        assert_eq!(parse_time("1 week").unwrap().1, Duration::from_secs(604800));
        assert_eq!(parse_time("1 month").unwrap().1, Duration::from_secs(2592000));
        assert_eq!(parse_time("1 year").unwrap().1, Duration::from_secs(31536000));
    }

    #[test]
    fn size_values() {
        assert_eq!(parse_size("1024").unwrap().1, 1024);
        assert_eq!(parse_size("1K").unwrap().1, 1000);
        assert_eq!(parse_size("1 KB").unwrap().1, 1000);
        assert_eq!(parse_size("1 KiB").unwrap().1, 1024);
        assert_eq!(parse_size("50G").unwrap().1, 50_000_000_000);
        assert_eq!(parse_size("50 GiB").unwrap().1, 50 * 1024 * 1024 * 1024);
        assert_eq!(parse_size("1M").unwrap().1, 1_000_000);
        assert_eq!(parse_size("1 MB").unwrap().1, 1_000_000);
    }

    #[test]
    fn quoted_strings() {
        assert_eq!(parse_quoted_string(r#""hello""#), Some("hello".into()));
        assert_eq!(parse_quoted_string(r#""with spaces""#), Some("with spaces".into()));
        assert_eq!(parse_quoted_string(r#""esc\"ape""#), Some(r#"esc"ape"#.into()));
        assert_eq!(parse_quoted_string("unquoted"), None);
    }

    #[test]
    fn split_number_unit_cases() {
        assert_eq!(split_number_unit("365 days"), ("365", "days"));
        assert_eq!(split_number_unit("50G"), ("50", "G"));
        assert_eq!(split_number_unit("1024"), ("1024", ""));
        assert_eq!(split_number_unit("1 KiB"), ("1", "KiB"));
    }
}
