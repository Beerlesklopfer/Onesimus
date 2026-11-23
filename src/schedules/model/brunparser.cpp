// SPDX-License-Identifier: AGPL-3.0-only
// SPDX-FileCopyrightText: 2026 Jörg Bernau
// SPDX-FileCopyrightText: 2000-2012 Free Software Foundation Europe e.V.
// SPDX-FileCopyrightText: 2010-2017 Planets Communications B.V.
// SPDX-FileCopyrightText: 2012-2026 Bareos GmbH & Co. KG
// Follows Bareos (see LICENSING.md); AGPL-3.0-only until reviewed.

/**
 * @file brunparser.cpp
 * @brief Reads a Bareos Run line (port of dird/run_conf.cc StoreRun())
 *
 * The structure follows StoreRun() and its Rust port step by step so that
 * the three can be compared: lexer, override loop, date loop.
 */

#include "schedules/model/brunspec.h"

#include <QRegularExpression>

// ============================================================================
// Model structures
// ============================================================================

BDateTimeMask BDateTimeMask::all()
{
    BDateTimeMask mask;
    mask.hour = AllHours;
    mask.mday = AllMdays;
    mask.month = AllMonths;
    mask.wday = AllWdays;
    mask.wom = AllWoms;
    mask.woy = AllWoys;
    mask.last7DaysOfMonth = false;
    return mask;
}

bool BDateTimeMask::operator==(const BDateTimeMask &other) const
{
    return hour == other.hour
        && mday == other.mday
        && month == other.month
        && wday == other.wday
        && wom == other.wom
        && woy == other.woy
        && last7DaysOfMonth == other.last7DaysOfMonth;
}

bool BRunOverrides::isEmpty() const
{
    return *this == BRunOverrides();
}

bool BRunOverrides::operator==(const BRunOverrides &other) const
{
    return level == other.level
        && pool == other.pool
        && fullPool == other.fullPool
        && incrementalPool == other.incrementalPool
        && differentialPool == other.differentialPool
        && nextPool == other.nextPool
        && storage == other.storage
        && messages == other.messages
        && maxRunSchedTime == other.maxRunSchedTime
        && priority == other.priority
        && spoolData == other.spoolData
        && accurate == other.accurate;
}

QList<int> BRunSpec::hours() const
{
    QList<int> result;
    for (int h = 0; h < 24; ++h) {
        if (mask.hour & (1u << h))
            result.append(h);
    }
    return result;
}

bool BRunSpec::operator==(const BRunSpec &other) const
{
    return overrides == other.overrides
        && mask == other.mask
        && minute == other.minute;
}

// ============================================================================
// Lexer (lib/lex.cc with the NoIdent option, for one line)
// ============================================================================

namespace {

struct Token {
    enum Type { Number, Str, Equals, Comma };
    Type type = Str;
    QString text;
    bool quoted = false;

    bool isWord() const { return type == Number || type == Str; }
};

bool isAsciiDigits(const QString &s)
{
    if (s.isEmpty())
        return false;
    for (const QChar c : s) {
        if (c < QLatin1Char('0') || c > QLatin1Char('9'))
            return false;
    }
    return true;
}

/// @return false with @p error set for a character sequence that is not a Run line
bool tokenize(const QString &input, QList<Token> *tokens, QString *error)
{
    const int n = input.size();
    int i = 0;
    while (i < n) {
        const QChar c = input[i];
        if (c == QLatin1Char('\n') || c == QLatin1Char('\r')) {
            *error = BRunParser::tr("A Run line cannot contain a line break.");
            return false;
        }
        if (c.isSpace()) {
            ++i;
            continue;
        }
        // A comment or the end of the directive
        if (c == QLatin1Char('#') || c == QLatin1Char(';'))
            break;
        if (c == QLatin1Char('=')) {
            tokens->append({Token::Equals, QStringLiteral("="), false});
            ++i;
            continue;
        }
        if (c == QLatin1Char(',')) {
            tokens->append({Token::Comma, QStringLiteral(","), false});
            ++i;
            continue;
        }
        if (c == QLatin1Char('{') || c == QLatin1Char('}') || c == QLatin1Char('@')) {
            *error = BRunParser::tr("Unexpected character \"%1\".").arg(c);
            return false;
        }
        if (c == QLatin1Char('"')) {
            // Quoted string; a backslash takes the next character as it is
            QString text;
            bool closed = false;
            ++i;
            while (i < n) {
                const QChar q = input[i];
                if (q == QLatin1Char('\n') || q == QLatin1Char('\r')) {
                    *error = BRunParser::tr("A Run line cannot contain a line break.");
                    return false;
                }
                if (q == QLatin1Char('\\')) {
                    if (i + 1 >= n)
                        break;
                    text.append(input[i + 1]);
                    i += 2;
                    continue;
                }
                ++i;
                if (q == QLatin1Char('"')) {
                    closed = true;
                    break;
                }
                text.append(q);
            }
            if (!closed) {
                *error = BRunParser::tr("A quoted value is not closed.");
                return false;
            }
            tokens->append({Token::Str, text, true});
            continue;
        }

        // A word runs up to white space or one of the separators
        const int start = i;
        while (i < n) {
            const QChar w = input[i];
            if (w.isSpace() || w == QLatin1Char('=') || w == QLatin1Char(',')
                || w == QLatin1Char(';') || w == QLatin1Char('#') || w == QLatin1Char('"')
                || w == QLatin1Char('{') || w == QLatin1Char('}')) {
                break;
            }
            ++i;
        }
        const QString word = input.mid(start, i - start);
        // Digits are a number only in front of white space, a comma or the end
        const bool numberEnd = (i >= n) || input[i].isSpace()
            || input[i] == QLatin1Char(',') || input[i] == QLatin1Char(';');
        const bool number = isAsciiDigits(word) && numberEnd;
        tokens->append({number ? Token::Number : Token::Str, word, false});
    }
    return true;
}

// ============================================================================
// Keyword tables (run_conf.cc keyw[] and RunFields[], dird_conf.cc joblevels[])
// ============================================================================

enum class State {
    None, Range, Mday, Month, Time, At, Wday, Daily, Weekly, Monthly, Hourly,
    Wom, Woy, Last, Modulo
};

struct Keyword {
    const char *name;
    State state;
    int code;
};

const Keyword keywords[] = {
    {"on", State::None, 0}, {"at", State::At, 0}, {"last", State::Last, 0},
    {"sun", State::Wday, 0}, {"mon", State::Wday, 1}, {"tue", State::Wday, 2},
    {"wed", State::Wday, 3}, {"thu", State::Wday, 4}, {"fri", State::Wday, 5},
    {"sat", State::Wday, 6},
    {"jan", State::Month, 0}, {"feb", State::Month, 1}, {"mar", State::Month, 2},
    {"apr", State::Month, 3}, {"may", State::Month, 4}, {"jun", State::Month, 5},
    {"jul", State::Month, 6}, {"aug", State::Month, 7}, {"sep", State::Month, 8},
    {"oct", State::Month, 9}, {"nov", State::Month, 10}, {"dec", State::Month, 11},
    {"sunday", State::Wday, 0}, {"monday", State::Wday, 1}, {"tuesday", State::Wday, 2},
    {"wednesday", State::Wday, 3}, {"thursday", State::Wday, 4}, {"friday", State::Wday, 5},
    {"saturday", State::Wday, 6},
    {"january", State::Month, 0}, {"february", State::Month, 1}, {"march", State::Month, 2},
    {"april", State::Month, 3}, {"june", State::Month, 5}, {"july", State::Month, 6},
    {"august", State::Month, 7}, {"september", State::Month, 8}, {"october", State::Month, 9},
    {"november", State::Month, 10}, {"december", State::Month, 11},
    {"daily", State::Daily, 0}, {"weekly", State::Weekly, 0}, {"monthly", State::Monthly, 0},
    {"hourly", State::Hourly, 0},
    {"1st", State::Wom, 0}, {"2nd", State::Wom, 1}, {"3rd", State::Wom, 2},
    {"4th", State::Wom, 3}, {"5th", State::Wom, 4},
    {"first", State::Wom, 0}, {"second", State::Wom, 1}, {"third", State::Wom, 2},
    {"fourth", State::Wom, 3}, {"fifth", State::Wom, 4},
};

const Keyword *findKeyword(const QString &word)
{
    for (const Keyword &k : keywords) {
        if (word.compare(QLatin1String(k.name), Qt::CaseInsensitive) == 0)
            return &k;
    }
    return nullptr;
}

enum class Field {
    Pool, FullPool, IncrementalPool, DifferentialPool, NextPool, Level, Storage,
    Messages, Priority, SpoolData, MaxRunSchedTime, Accurate
};

struct FieldName {
    const char *name;
    Field field;
};

const FieldName runFields[] = {
    {"pool", Field::Pool}, {"fullpool", Field::FullPool},
    {"incrementalpool", Field::IncrementalPool},
    {"differentialpool", Field::DifferentialPool}, {"nextpool", Field::NextPool},
    {"level", Field::Level}, {"storage", Field::Storage}, {"messages", Field::Messages},
    {"priority", Field::Priority}, {"spooldata", Field::SpoolData},
    {"maxrunschedtime", Field::MaxRunSchedTime}, {"accurate", Field::Accurate},
};

const FieldName *findField(const QString &word)
{
    for (const FieldName &f : runFields) {
        if (word.compare(QLatin1String(f.name), Qt::CaseInsensitive) == 0)
            return &f;
    }
    return nullptr;
}

// ============================================================================
// Helpers
// ============================================================================

/// Digits only, as a number; -1 if empty, not digits or too large
int digitsToInt(const QString &s)
{
    if (!isAsciiDigits(s))
        return -1;
    bool ok = false;
    const int value = s.toInt(&ok);
    return ok ? value : -1;
}

bool isWeek(const QString &s)
{
    return s.size() == 3 && (s[0] == QLatin1Char('w') || s[0] == QLatin1Char('W'))
        && isAsciiDigits(s.mid(1));
}

template <typename T>
void setRange(T &mask, int from, int to)
{
    for (int i = from; i <= to; ++i)
        mask = T(mask | (T(1) << i));
}

/// from..to, around the end at @p max if from is not below to (run_conf.cc)
template <typename T>
void setWrappingRange(T &mask, int from, int to, int max)
{
    if (from < to) {
        setRange(mask, from, to);
    } else {
        setRange(mask, from, max);
        setRange(mask, 0, to);
    }
}

std::optional<bool> yesNo(const QString &word)
{
    const QString lower = word.toLower();
    if (lower == QLatin1String("yes") || lower == QLatin1String("true"))
        return true;
    if (lower == QLatin1String("no") || lower == QLatin1String("false"))
        return false;
    return std::nullopt;
}

/// A Bareos time period: numbers, each with an optional unit (lib/edit.cc)
bool isTimePeriod(const QString &text)
{
    static const QRegularExpression rx(
        QStringLiteral("^\\s*(?:\\d+(?:\\.\\d+)?\\s*([A-Za-z]*)\\s*)+$"));
    static const QRegularExpression part(
        QStringLiteral("\\d+(?:\\.\\d+)?\\s*([A-Za-z]*)"));
    static const QStringList units = {
        QStringLiteral("n"), QStringLiteral("seconds"), QStringLiteral("months"),
        QStringLiteral("minutes"), QStringLiteral("mins"), QStringLiteral("hours"),
        QStringLiteral("days"), QStringLiteral("weeks"), QStringLiteral("quarters"),
        QStringLiteral("years")
    };
    if (!rx.match(text).hasMatch())
        return false;
    auto it = part.globalMatch(text);
    while (it.hasNext()) {
        const QString unit = it.next().captured(1).toLower();
        if (unit.isEmpty())
            continue;
        bool known = false;
        for (const QString &u : units) {
            if (u.startsWith(unit)) {
                known = true;
                break;
            }
        }
        if (!known)
            return false;
    }
    return true;
}

} // namespace

// ============================================================================
// BRunParser
// ============================================================================

const QStringList &BRunParser::levelNames()
{
    static const QStringList names = {
        QStringLiteral("Full"), QStringLiteral("Base"), QStringLiteral("Incremental"),
        QStringLiteral("Differential"), QStringLiteral("Since"),
        QStringLiteral("VirtualFull"), QStringLiteral("Catalog"),
        QStringLiteral("InitCatalog"), QStringLiteral("VolumeToCatalog"),
        QStringLiteral("DiskToCatalog"), QStringLiteral("Data")
    };
    return names;
}

QString BRunParser::canonicalLevel(const QString &word)
{
    for (const QString &name : levelNames()) {
        if (name.compare(word, Qt::CaseInsensitive) == 0)
            return name;
    }
    return QString();
}

BRunLine BRunParser::parse(const QString &text)
{
    BRunLine line;
    line.original = text;

    auto fail = [&line](const QString &message) {
        line.spec.reset();
        line.needsRewrite = false;
        line.parseError = message;
        return line;
    };

    QList<Token> tokens;
    QString lexError;
    if (!tokenize(text, &tokens, &lexError))
        return fail(lexError);
    if (tokens.isEmpty())
        return fail(tr("The Run line is empty."));

    BRunSpec spec;
    BRunOverrides &o = spec.overrides;

    // --- Job level "full", "incremental", ... and keyword=value pairs ---
    int pos = 0;
    while (pos < tokens.size()) {
        const Token &keyToken = tokens[pos];
        if (!keyToken.isWord())
            break;
        const QString key = keyToken.text;

        if (const FieldName *field = findField(key)) {
            if (pos + 1 >= tokens.size() || tokens[pos + 1].type != Token::Equals) {
                return fail(tr("Expected an equals, got: %1")
                                .arg(pos + 1 < tokens.size() ? tokens[pos + 1].text : QString()));
            }
            if (pos + 2 >= tokens.size() || !tokens[pos + 2].isWord()) {
                return fail(tr("Expected a value after \"%1=\".").arg(key));
            }
            const Token &valueToken = tokens[pos + 2];
            const QString value = valueToken.text;
            pos += 3;

            switch (field->field) {
            case Field::SpoolData:
            case Field::Accurate: {
                const std::optional<bool> flag = yesNo(value);
                if (!flag)
                    return fail(tr("Expect a YES or NO, got: %1").arg(value));
                if (field->field == Field::SpoolData)
                    o.spoolData = flag;
                else
                    o.accurate = flag;
                break;
            }
            case Field::Level: {
                const QString level = canonicalLevel(value);
                if (level.isEmpty())
                    return fail(tr("Job level field: %1 not found in run record").arg(value));
                o.level = level;
                break;
            }
            case Field::Priority: {
                const int priority = (valueToken.type == Token::Number) ? digitsToInt(value) : -1;
                if (priority < 0)
                    return fail(tr("expected a positive integer, got: %1").arg(value));
                o.priority = priority;
                break;
            }
            case Field::MaxRunSchedTime:
                if (!isTimePeriod(value))
                    return fail(tr("expected a time period, got: %1").arg(value));
                o.maxRunSchedTime = value;
                break;
            case Field::Pool:
            case Field::FullPool:
            case Field::IncrementalPool:
            case Field::DifferentialPool:
            case Field::NextPool:
            case Field::Storage:
            case Field::Messages:
                if (value.isEmpty())
                    return fail(tr("Expected a value after \"%1=\".").arg(key));
                if (field->field == Field::Pool) o.pool = value;
                else if (field->field == Field::FullPool) o.fullPool = value;
                else if (field->field == Field::IncrementalPool) o.incrementalPool = value;
                else if (field->field == Field::DifferentialPool) o.differentialPool = value;
                else if (field->field == Field::NextPool) o.nextPool = value;
                else if (field->field == Field::Storage) o.storage = value;
                else o.messages = value;
                break;
            }
            continue;
        }

        // Old style job levels without keyword
        const QString level = canonicalLevel(key);
        if (!level.isEmpty()) {
            o.level = level;
            ++pos;
            continue;
        }
        break;
    }

    // --- Date and time; defaults: every hour of every day, minute 0 ---
    BDateTimeMask &m = spec.mask;
    const int dateStart = pos;

    for (; pos < tokens.size(); ++pos) {
        const Token &token = tokens[pos];
        const QString &word = token.text;
        State state = State::None;
        int code = 0;

        switch (token.type) {
        case Token::Comma:
            // Bareos expects a word directly behind the overrides
            if (pos == dateStart)
                line.needsRewrite = true;
            continue;
        case Token::Equals:
            return fail(tr("Unexpected token: %1").arg(word));
        case Token::Number:
            state = State::Mday;
            code = digitsToInt(word) - 1;
            if (code < 0 || code > 30)
                return fail(tr("Day number out of range (1-31)"));
            // Printed like this by the Director, but read back only as "on 1"
            if (pos == dateStart)
                line.needsRewrite = true;
            break;
        case Token::Str:
            if (token.quoted)
                return fail(tr("Unexpected token: \"%1\"").arg(word));
            if (word.contains(QLatin1Char('-'))) {
                state = State::Range;
            } else if (word.contains(QLatin1Char(':'))) {
                state = State::Time;
            } else if (word.contains(QLatin1Char('/'))) {
                state = State::Modulo;
            } else if (isWeek(word)) {
                code = digitsToInt(word.mid(1));
                if (code < 0 || code > 53)
                    return fail(tr("Week number out of range (0-53)"));
                state = State::Woy;
            } else if (const Keyword *k = findKeyword(word)) {
                state = k->state;
                code = k->code;
            } else {
                return fail(tr("Job type field: %1 in run record not found").arg(word));
            }
            break;
        }

        switch (state) {
        case State::None:
            continue;
        case State::Mday:
            if (!spec.haveMday) { m.mday = 0; spec.haveMday = true; }
            m.mday |= (1u << code);
            break;
        case State::Month:
            if (!spec.haveMonth) { m.month = 0; spec.haveMonth = true; }
            m.month |= quint16(1u << code);
            break;
        case State::Wday:
            if (!spec.haveWday) { m.wday = 0; spec.haveWday = true; }
            m.wday |= quint8(1u << code);
            break;
        case State::Wom:
            if (!spec.haveWom) { m.wom = 0; spec.haveWom = true; }
            m.wom |= quint8(1u << code);
            break;
        case State::Woy:
            if (!spec.haveWoy) { m.woy = 0; spec.haveWoy = true; }
            m.woy |= (Q_UINT64_C(1) << code);
            break;
        case State::Time: {
            if (!spec.haveAt)
                return fail(tr("Time must be preceded by keyword AT."));
            const int colon = word.indexOf(QLatin1Char(':'));
            const QString hourText = word.left(colon);
            const QString rest = word.mid(colon + 1);
            const QString minuteText = rest.left(2);
            const QString suffix = rest.mid(2);
            bool pm = false;
            bool am = false;
            if (suffix.compare(QLatin1String("pm"), Qt::CaseInsensitive) == 0) {
                pm = true;
            } else if (suffix.compare(QLatin1String("am"), Qt::CaseInsensitive) == 0) {
                am = true;
            } else if (suffix.compare(QLatin1String("at"), Qt::CaseInsensitive) == 0) {
                // "at 00:05at 01:05": how the Director prints several hours
                line.needsRewrite = true;
            } else if (!suffix.isEmpty()) {
                return fail(tr("Bad time specification."));
            }
            int hour = digitsToInt(hourText);
            const int minute = (minuteText.size() == 2) ? digitsToInt(minuteText) : -1;
            if (hour < 0 || minute < 0)
                return fail(tr("Bad time specification."));
            // 12am is 00:00 and 12pm is 12:00
            if (pm) {
                if (hour != 12)
                    hour += 12;
            } else if (am && hour == 12) {
                hour -= 12;
            }
            if (hour > 23 || minute > 59)
                return fail(tr("Bad time specification."));
            if (!spec.haveHour)
                m.hour = 0;
            m.hour |= (1u << hour);
            spec.minute = minute;
            spec.haveHour = true;
            break;
        }
        case State::At:
            spec.haveAt = true;
            break;
        case State::Last:
            m.last7DaysOfMonth = true;
            if (!spec.haveWom) { m.wom = 0; spec.haveWom = true; }
            break;
        case State::Modulo: {
            const int slash = word.indexOf(QLatin1Char('/'));
            const QString left = word.left(slash);
            const QString right = word.mid(slash + 1);
            if (isAsciiDigits(left) && isAsciiDigits(right)) {
                // Day modulo: <first day>/<every n days>
                code = digitsToInt(left) - 1;
                const int code2 = digitsToInt(right);
                // Bareos divides by an interval of 0; it is rejected here
                if (code < 0 || code > 30 || code2 < 1 || code2 > 30)
                    return fail(tr("Bad day specification in modulo."));
                if (code > code2)
                    return fail(tr("Bad day specification, offset must always be <= than modulo."));
                if (!spec.haveMday) { m.mday = 0; spec.haveMday = true; }
                for (int i = 0; i < 31; ++i) {
                    if (i % code2 == 0 && i + code <= 30)
                        m.mday |= (1u << (i + code));
                }
            } else if (isWeek(left) && isWeek(right)) {
                code = digitsToInt(left.mid(1));
                const int code2 = digitsToInt(right.mid(1));
                if (code < 0 || code > 53)
                    return fail(tr("Week number out of range (0-53) in modulo"));
                if (code2 <= 0 || code2 > 53)
                    return fail(tr("Week interval out of range (1-53) in modulo"));
                if (code > code2) {
                    return fail(tr("Bad week number specification in modulo, offset must "
                                   "always be <= than modulo."));
                }
                if (!spec.haveWoy) { m.woy = 0; spec.haveWoy = true; }
                for (int week = code; week <= 53; week += code2)
                    m.woy |= (Q_UINT64_C(1) << week);
            } else {
                return fail(tr("Bad modulo time specification. Format for weekdays is "
                               "'01/02', for yearweeks is 'w01/w02'."));
            }
            break;
        }
        case State::Range: {
            const int dash = word.indexOf(QLatin1Char('-'));
            const QString left = word.left(dash);
            const QString right = word.mid(dash + 1);
            if (isAsciiDigits(left) && isAsciiDigits(right)) {
                code = digitsToInt(left) - 1;
                const int code2 = digitsToInt(right) - 1;
                if (code < 0 || code > 30 || code2 < 0 || code2 > 30)
                    return fail(tr("Bad day range specification."));
                if (!spec.haveMday) { m.mday = 0; spec.haveMday = true; }
                setWrappingRange(m.mday, code, code2, 30);
            } else if (isWeek(left) && isWeek(right)) {
                code = digitsToInt(left.mid(1));
                const int code2 = digitsToInt(right.mid(1));
                if (code < 0 || code > 53 || code2 < 0 || code2 > 53)
                    return fail(tr("Week number out of range (0-53)"));
                if (!spec.haveWoy) { m.woy = 0; spec.haveWoy = true; }
                setWrappingRange(m.woy, code, code2, 53);
            } else {
                // Week days, months or positions
                const Keyword *first = findKeyword(left);
                if (!first || (first->state != State::Month && first->state != State::Wday
                               && first->state != State::Wom)) {
                    return fail(tr("Invalid month, week or position day range"));
                }
                const Keyword *second = findKeyword(right);
                if (!second || second->state != first->state || second->code == first->code)
                    return fail(tr("Invalid month, weekday or position range"));
                if (first->state == State::Wday) {
                    if (!spec.haveWday) { m.wday = 0; spec.haveWday = true; }
                    setWrappingRange(m.wday, first->code, second->code, 6);
                } else if (first->state == State::Month) {
                    if (!spec.haveMonth) { m.month = 0; spec.haveMonth = true; }
                    setWrappingRange(m.month, first->code, second->code, 11);
                } else {
                    if (!spec.haveWom) { m.wom = 0; spec.haveWom = true; }
                    setWrappingRange(m.wom, first->code, second->code, 4);
                }
            }
            break;
        }
        case State::Hourly:
            spec.haveHour = true;
            m.hour = BDateTimeMask::AllHours;
            break;
        case State::Weekly:
            spec.haveMday = spec.haveWom = spec.haveWoy = true;
            m.mday = BDateTimeMask::AllMdays;
            m.wom = BDateTimeMask::AllWoms;
            m.woy = BDateTimeMask::AllWoys;
            break;
        case State::Daily:
            spec.haveMday = true;
            m.wday = BDateTimeMask::AllWdays;
            break;
        case State::Monthly:
            spec.haveMonth = true;
            m.month = BDateTimeMask::AllMonths;
            break;
        }
    }

    line.spec = spec;
    return line;
}
