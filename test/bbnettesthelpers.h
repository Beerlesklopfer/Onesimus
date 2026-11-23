// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

/**
 * @file bbnettesthelpers.h
 * @brief BNET pieces for fake Directors in tests: CRAM-MD5 answer, framing
 */
#ifndef BBNETTESTHELPERS_H
#define BBNETTESTHELPERS_H

#include <QByteArray>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include <QString>
#include <QtEndian>

namespace BFakeBnet {

/// Bareos BNET signals (bsock.h) used by the fake
enum Signal : qint32 {
    Eod = -1,
    Terminate = -4,
    CmdOk = -15,
    CmdBegin = -16,
    MainPrompt = -18,
};

inline QByteArray keyOf(const QString &password)
{
    return QCryptographicHash::hash(password.toLatin1(), QCryptographicHash::Md5).toHex();
}

/// Bareos BinToBase64() (lib/base64.cc), both variants
inline QByteArray bareosBase64(const QByteArray &bin, bool compatible)
{
    static const char digits[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    QByteArray out;
    quint32 reg = 0;
    int rem = 0;
    for (int i = 0; i < bin.size();) {
        if (rem < 6) {
            reg <<= 8;
            if (compatible)
                reg |= quint8(bin[i++]);
            else
                reg |= quint32(qint32(qint8(bin[i++])));
            rem += 8;
        }
        const quint32 save = reg;
        reg >>= (rem - 6);
        out.append(digits[reg & 0x3F]);
        reg = save;
        rem -= 6;
    }
    if (rem) {
        const quint32 mask = (1u << rem) - 1;
        // Bareos quirk: only the compatible variant shifts the last digit
        out.append(digits[compatible ? (reg & mask) << (6 - rem) : (reg & mask)]);
    }
    return out;
}

inline QByteArray hmacAnswer(const QByteArray &challenge, const QString &password, bool compatible)
{
    return bareosBase64(QMessageAuthenticationCode::hash(challenge, keyOf(password),
                                                         QCryptographicHash::Md5),
                        compatible);
}

/// One data telegram: signed 32-bit big-endian length, then the payload
inline QByteArray frame(const QByteArray &payload)
{
    QByteArray out(4, '\0');
    qToBigEndian<qint32>(qint32(payload.size()), reinterpret_cast<uchar *>(out.data()));
    return out + payload;
}

/// A signal: the negative length alone
inline QByteArray signal(qint32 value)
{
    QByteArray out(4, '\0');
    qToBigEndian<qint32>(value, reinterpret_cast<uchar *>(out.data()));
    return out;
}

} // namespace BFakeBnet

#endif // BBNETTESTHELPERS_H
