/**
 * @file bnetframe.h
 * @brief Decision on the 4-byte length header of a BNET telegram
 *
 * Every BNET telegram starts with a signed 32-bit big-endian length. A
 * negative value is a signal (EOD, MAIN_PROMPT, ...) without payload; any
 * other value announces that many payload bytes. The value comes from the
 * network, so it is bounded before anything is buffered for it.
 */
#ifndef BNETFRAME_H
#define BNETFRAME_H

#include <QtEndian>
#include <QtGlobal>

namespace BnetFrame {

/// Bytes of the length header
constexpr qint64 HeaderSize = 4;

/// Largest payload accepted from an authenticated Director. Bareos itself
/// refuses telegrams over 1 000 000 bytes and splits long replies (file
/// listings, job logs) into many telegrams, so this is generous.
constexpr qint64 MaxSessionPayload = 16 * 1024 * 1024;

/// Largest payload accepted during authentication (challenge, "1000 OK auth",
/// banner: all well below 1 KiB)
constexpr qint64 MaxAuthPayload = 4096;

enum class HeaderKind {
    Signal,     ///< negative length: BNET signal, header only
    Data,       ///< payload of the announced length follows
    TooLarge    ///< announced length exceeds the limit: protocol violation
};

/// Length announced by the first four bytes of @p data (at least 4 bytes)
inline qint32 announcedLength(const char *data)
{
    return qFromBigEndian<qint32>(reinterpret_cast<const uchar *>(data));
}

/// What a header with this length means under the given payload limit
inline HeaderKind classifyHeader(qint32 length, qint64 maxPayload)
{
    if (length < 0) {
        return HeaderKind::Signal;
    }
    if (qint64(length) > maxPayload) {
        return HeaderKind::TooLarge;
    }
    return HeaderKind::Data;
}

/// Bytes a data telegram occupies in the receive buffer (header + payload),
/// in 64 bit so that a length near INT_MAX cannot overflow
inline qint64 telegramSize(qint32 length)
{
    return HeaderSize + qint64(length);
}

} // namespace BnetFrame

#endif // BNETFRAME_H
