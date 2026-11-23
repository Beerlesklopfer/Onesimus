// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

#include "bbyteformat.h"

#include <cmath>

namespace BByteFormat {

namespace {

// "50.0" -> "50", "1.50" -> "1.5"
QString withoutTrailingZeros(QString number)
{
    if (number.contains(QLatin1Char('.'))) {
        while (number.endsWith(QLatin1Char('0'))) {
            number.chop(1);
        }
        if (number.endsWith(QLatin1Char('.'))) {
            number.chop(1);
        }
    }
    return number;
}

} // namespace

QString format(qint64 bytes, int decimals)
{
    static const char *const units[] = {"KiB", "MiB", "GiB", "TiB", "PiB", "EiB"};
    static constexpr int unitCount = int(sizeof(units) / sizeof(units[0]));

    // The magnitude as unsigned: -INT64_MIN does not fit into qint64
    const bool negative = bytes < 0;
    const quint64 magnitude = negative ? quint64(0) - quint64(bytes) : quint64(bytes);
    const QString sign = negative ? QStringLiteral("-") : QString();

    if (magnitude < 1024) {
        return sign + QString::number(magnitude) + QStringLiteral(" B");
    }

    decimals = qBound(0, decimals, 3);
    const double factor = std::pow(10.0, decimals);

    int unit = 0;
    double divisor = 1024.0;
    while (unit + 1 < unitCount && double(magnitude) >= divisor * 1024.0) {
        divisor *= 1024.0;
        ++unit;
    }
    double value = std::round(double(magnitude) / divisor * factor) / factor;
    // 1048575 bytes are 1023.999 KiB: rounded that is 1024 KiB, shown as 1 MiB
    if (value >= 1024.0 && unit + 1 < unitCount) {
        ++unit;
        divisor *= 1024.0;
        value = std::round(double(magnitude) / divisor * factor) / factor;
    }

    const QString number = withoutTrailingZeros(QString::number(value, 'f', decimals));
    return sign + number + QLatin1Char(' ') + QLatin1String(units[unit]);
}

QString formatDecimal(quint64 bytes, int decimals)
{
    static const char *const units[] = {"kB", "MB", "GB", "TB", "PB", "EB"};
    static constexpr int unitCount = int(sizeof(units) / sizeof(units[0]));

    if (bytes < 1000) {
        return QString::number(bytes) + QStringLiteral(" B");
    }

    decimals = qBound(0, decimals, 3);
    const double factor = std::pow(10.0, decimals);

    int unit = 0;
    double divisor = 1000.0;
    while (unit + 1 < unitCount && double(bytes) >= divisor * 1000.0) {
        divisor *= 1000.0;
        ++unit;
    }
    double value = std::round(double(bytes) / divisor * factor) / factor;
    // 999999 bytes are 999.999 kB: rounded that is 1000 kB, shown as 1 MB
    if (value >= 1000.0 && unit + 1 < unitCount) {
        ++unit;
        divisor *= 1000.0;
        value = std::round(double(bytes) / divisor * factor) / factor;
    }

    const QString number = withoutTrailingZeros(QString::number(value, 'f', decimals));
    return number + QLatin1Char(' ') + QLatin1String(units[unit]);
}

bool parseBareosSize(const QString &text, quint64 *bytes)
{
    struct Suffix {
        const char *name;
        double multiplier;
    };
    // Longest names first, so "kb" is not read as "k" followed by junk
    static const Suffix suffixes[] = {
        {"kb", 1e3},  {"mb", 1e6},  {"gb", 1e9},  {"tb", 1e12}, {"pb", 1e15}, {"eb", 1e18},
        {"k", 1024.0}, {"m", 1048576.0}, {"g", 1073741824.0}, {"t", 1099511627776.0},
        {"p", 1125899906842624.0}, {"e", 1152921504606846976.0},
    };

    const QString s = text.trimmed().toLower();
    if (s.isEmpty()) {
        return false;
    }

    double total = 0.0;
    int pos = 0;
    const int length = s.size();
    bool any = false;
    while (pos < length) {
        while (pos < length && s.at(pos).isSpace()) {
            ++pos;
        }
        if (pos >= length) {
            break;
        }
        // The number: digits with an optional fraction
        const int numberStart = pos;
        while (pos < length && (s.at(pos).isDigit() || s.at(pos) == QLatin1Char('.'))) {
            ++pos;
        }
        bool ok = false;
        const double number = s.mid(numberStart, pos - numberStart).toDouble(&ok);
        if (!ok) {
            return false;
        }
        while (pos < length && s.at(pos).isSpace()) {
            ++pos;
        }
        // The suffix: letters up to the next digit or space; none = bytes
        const int suffixStart = pos;
        while (pos < length && s.at(pos).isLetter()) {
            ++pos;
        }
        const QString suffix = s.mid(suffixStart, pos - suffixStart);
        double multiplier = 1.0;
        if (!suffix.isEmpty()) {
            bool known = false;
            for (const Suffix &candidate : suffixes) {
                if (suffix == QLatin1String(candidate.name)) {
                    multiplier = candidate.multiplier;
                    known = true;
                    break;
                }
            }
            if (!known) {
                return false;
            }
        }
        total += number * multiplier;
        any = true;
    }
    if (!any || total < 0.0 || total >= 18446744073709551616.0) {
        return false;
    }
    if (bytes) {
        *bytes = quint64(total);
    }
    return true;
}

} // namespace BByteFormat
