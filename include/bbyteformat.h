#ifndef BBYTEFORMAT_H
#define BBYTEFORMAT_H

#include <QString>
#include <QtGlobal>

/**
 * @brief Byte counts as text, in binary units with IEC names
 *
 * One unit for the whole application: powers of 1024, named KiB, MiB, GiB,
 * TiB, PiB and EiB. Bareos reads the suffixes k, m, g, t of a size directive
 * as binary and kb, mb, gb, tb as decimal (core/src/lib/edit.cc,
 * size_to_uint64), so "Maximum Volume Bytes = 50G" is 50 GiB and appears here
 * as "50 GiB", exactly as configured. A decimal "53.7 GB" would look like an
 * overflow of the limit. There is no decimal mode.
 *
 * The decimal separator is always '.', whatever the locale, so that the text
 * is the same in every language and in tests.
 *
 * One exception, formatDecimal(): the free tier limit of the Onesimus
 * Director is defined in decimal bytes (1 TB = 10^12 bytes, as the Director
 * prints it in `status dir`). Shown in binary units it would read
 * "931.3 GiB", so usage against that limit is shown in decimal units.
 *
 * @since 2.9
 */
namespace BByteFormat {

/**
 * @brief @p bytes as "512 B", "1.5 KiB", "50 GiB", ...
 *
 * Below 1024 the count is printed in bytes. Above, the largest unit whose
 * value is at least 1 is used, with @p decimals digits after the point;
 * trailing zeros are dropped ("50 GiB", not "50.0 GiB"). A value that rounds
 * up to 1024 of a unit is shown in the next unit ("1 MiB", not "1024 KiB").
 * Negative values get a leading minus.
 */
QString format(qint64 bytes, int decimals = 1);

/**
 * @brief @p bytes in decimal units: "512 B", "73.6 GB", "1 TB", ...
 *
 * Powers of 1000, named kB, MB, GB, TB, PB, EB. Otherwise as format():
 * the largest unit whose value is at least 1, @p decimals digits, trailing
 * zeros dropped, a value that rounds up to 1000 shown in the next unit.
 * Only for amounts that are defined in decimal bytes (the free tier limit).
 */
QString formatDecimal(quint64 bytes, int decimals = 1);

/**
 * @brief Parses a size as a Bareos resource directive writes it
 *
 * Accepted: a number (bytes), or one or more numbers each followed by a
 * suffix, as in "50G", "50 g", "1.5 TB", "1g 512m". The suffixes are those of
 * Bareos' size_to_uint64 (core/src/lib/edit.cc): k, m, g, t, p, e are binary
 * (1024^n), kb, mb, gb, tb, pb, eb decimal (1000^n), case-insensitive.
 *
 * @param text   The directive value
 * @param bytes  Receives the size in bytes on success
 * @return false for an empty string, an unknown suffix or trailing junk
 */
bool parseBareosSize(const QString &text, quint64 *bytes);

} // namespace BByteFormat

#endif // BBYTEFORMAT_H
