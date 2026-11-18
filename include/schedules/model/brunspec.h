#ifndef BRUNSPEC_H
#define BRUNSPEC_H

#include <QCoreApplication>
#include <QList>
#include <QString>
#include <QStringList>

#include <optional>

/**
 * @file brunspec.h
 * @brief Model of a Bareos Schedule "Run" directive
 *
 * A faithful model of what the Director keeps for one Run line: the
 * overrides and six bit masks with a minute (dird/run_conf.cc StoreRun()).
 * QtCore only; no widgets and no Director.
 *
 * Rules that hold everywhere this model is used:
 *  - A line the parser does not fully understand has no BRunSpec. It is kept
 *    verbatim with the reason and is never rewritten.
 *  - A line without a level has no level (the job's own level applies).
 *  - An untouched line is written back byte-identical from its original text.
 *  - A touched line is generated, parsed again and compared with what was
 *    intended; a difference stops the write.
 */

/// When a run triggers: date_time_mask of Bareos, one bit per unit
struct BDateTimeMask
{
    quint32 hour = 0;   ///< 24 bits, bit 0 = 00:xx
    quint32 mday = 0;   ///< 31 bits, bit 0 = the 1st
    quint16 month = 0;  ///< 12 bits, bit 0 = January
    quint8 wday = 0;    ///< 7 bits, bit 0 = Sunday
    quint8 wom = 0;     ///< 5 bits, bit 0 = first week of the month
    quint64 woy = 0;    ///< 54 bits, bit 0 = w00
    bool last7DaysOfMonth = false;  ///< "last": the last seven days of a month

    static constexpr quint32 AllHours = (1u << 24) - 1;
    static constexpr quint32 AllMdays = (1u << 31) - 1;
    static constexpr quint16 AllMonths = (1u << 12) - 1;
    static constexpr quint8 AllWdays = (1u << 7) - 1;
    static constexpr quint8 AllWoms = (1u << 5) - 1;
    static constexpr quint64 AllWoys = (Q_UINT64_C(1) << 54) - 1;

    /// Every hour of every day: what a Run line without any date or time says
    static BDateTimeMask all();

    bool operator==(const BDateTimeMask &other) const;
    bool operator!=(const BDateTimeMask &other) const { return !(*this == other); }
};

/// The job settings a Run line may override; each one is optional
struct BRunOverrides
{
    std::optional<QString> level;   ///< In the spelling of Bareos ("Full", "VirtualFull", ...)
    std::optional<QString> pool;
    std::optional<QString> fullPool;
    std::optional<QString> incrementalPool;
    std::optional<QString> differentialPool;
    std::optional<QString> nextPool;
    std::optional<QString> storage;
    std::optional<QString> messages;
    std::optional<QString> maxRunSchedTime;  ///< A Bareos time period, as written
    std::optional<int> priority;
    std::optional<bool> spoolData;
    std::optional<bool> accurate;

    bool isEmpty() const;
    bool operator==(const BRunOverrides &other) const;
    bool operator!=(const BRunOverrides &other) const { return !(*this == other); }
};

/// One understood Run line
struct BRunSpec
{
    BRunOverrides overrides;
    BDateTimeMask mask = BDateTimeMask::all();
    int minute = 0;

    // Which parts the line named (have_* in run_conf.cc). They record how the
    // line was written, not when it runs, and are not part of the comparison.
    bool haveAt = false;
    bool haveHour = false;
    bool haveWday = false;
    bool haveMday = false;
    bool haveMonth = false;
    bool haveWom = false;
    bool haveWoy = false;

    /// The set hours in ascending order
    QList<int> hours() const;

    /// Same overrides, same masks, same minute
    bool operator==(const BRunSpec &other) const;
    bool operator!=(const BRunSpec &other) const { return !(*this == other); }
};

/// One Run line of a schedule, understood or not
struct BRunLine
{
    QString original;               ///< As reported by the Director; empty for a new line
    std::optional<BRunSpec> spec;   ///< Empty: not understood
    QString parseError;             ///< Why it is not understood
    bool dirty = false;             ///< Changed by the user: written from spec, not from original

    /// Understood, but in a spelling Bareos prints and does not read back
    /// (see BRunParser). Such a line is written from spec even if untouched.
    bool needsRewrite = false;

    bool understood() const { return spec.has_value(); }
};

struct BSchedule
{
    QString name;
    QString description;
    bool enabled = true;
    QList<BRunLine> runs;
};

/**
 * @brief Reads a Run line: a token-stream port of Bareos' StoreRun()
 *
 * The grammar is the one of dird/run_conf.cc and of its Rust port
 * (onesimus-dird, crates/bareos-config/src/parser.rs store_run()): first the
 * overrides (keyword=value) and old-style bare levels, then the date and
 * time words with their have_* rules.
 *
 * Nothing is guessed. An unknown level, an unknown word, a time without
 * "at" or any other form that is not fully understood gives a BRunLine
 * without a spec and with the reason in parseError.
 *
 * Two spellings are read although Bareos itself rejects them, because the
 * Director prints them in "show schedules" (dird_conf.cc PrintConfigRun()):
 * a day of the month directly behind the overrides ("Full 1 at 02:05",
 * Bareos wants "on 1") and the times of a line with several hours glued
 * together ("at 00:05at 01:05"). Both set BRunLine::needsRewrite.
 */
class BRunParser
{
    Q_DECLARE_TR_FUNCTIONS(BRunParser)

public:
    static BRunLine parse(const QString &text);

    /// The job levels of Bareos in its own spelling
    static const QStringList &levelNames();

    /// @return the level in the spelling of Bareos, empty if @p word is no level
    static QString canonicalLevel(const QString &word);
};

/**
 * @brief Writes a Run line in canonical form, overrides first
 *
 *     Level=X Pool=Y ... <months> <weeks of year> <weeks of month> <days>
 *     on <days of month> at HH:MM
 *
 * All 24 hours become "hourly at 0:MM"; fewer hours become one "at HH:MM"
 * each on the same line.
 */
class BRunWriter
{
    Q_DECLARE_TR_FUNCTIONS(BRunWriter)

public:
    /// @return the canonical text, or an empty string with @p error set if
    ///         the spec cannot be written (an empty mask, a bad minute)
    static QString write(const BRunSpec &spec, QString *error = nullptr);

    /**
     * @brief The text to write for one line
     *
     * The original text for an untouched line. A touched line is generated,
     * parsed again and compared with its spec.
     * @return an empty string with @p error set if the line is not
     *         understood or the check fails
     */
    static QString lineText(const BRunLine &line, QString *error = nullptr);

    /**
     * @brief The Run texts of a whole schedule, or nothing at all
     * @return false with @p error naming the line if any line cannot be written
     */
    static bool runTexts(const QList<BRunLine> &runs, QStringList *texts, QString *error = nullptr);

    /// The same for Run lines as they were reported or read from a file
    static bool runTexts(const QStringList &reported, QStringList *texts, QString *error = nullptr);

    /// "1st", "2nd-5th", "1st,3rd last", ...; empty if every week is set
    static QString weekOfMonthText(const BDateTimeMask &mask);

    /// A Run text as the value of run="..." in a "configure add" command
    static QString quotedForCommand(const QString &runText);
};

#endif // BRUNSPEC_H
