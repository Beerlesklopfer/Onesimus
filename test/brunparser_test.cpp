/**
 * @file brunparser_test.cpp
 * @brief The model of a Bareos Run line: parser, writer, round trips
 *
 * The expected masks come from three places, named at each group:
 *  - the examples of the Schedule resource in the Bareos documentation
 *    (docs.bareos.org, Configuration, Director), worked out by hand from
 *    the rules of dird/run_conf.cc StoreRun();
 *  - the tests of the Rust port (onesimus-dird,
 *    crates/bareos-config/src/parser.rs, run_masks_like_store_run());
 *  - the failure cases of the review of the old substring parser.
 *
 * Bit 0 is Sunday, January, the 1st, w00, the first week of the month.
 * Runs without a Director (registered with CTest).
 */

#include <QtTest>

#include "schedules/model/brunspec.h"

Q_DECLARE_METATYPE(BRunSpec)

namespace {

enum Wday { Sun = 0, Mon, Tue, Wed, Thu, Fri, Sat };

quint64 bits(std::initializer_list<int> positions)
{
    quint64 mask = 0;
    for (int p : positions)
        mask |= (Q_UINT64_C(1) << p);
    return mask;
}

quint64 bitRange(int from, int to)
{
    quint64 mask = 0;
    for (int i = from; i <= to; ++i)
        mask |= (Q_UINT64_C(1) << i);
    return mask;
}

/// A run at one time of day, everything else at its default
BRunSpec at(int hour, int minute)
{
    BRunSpec spec;
    spec.mask.hour = quint32(1u << hour);
    spec.minute = minute;
    return spec;
}

BRunSpec level(BRunSpec spec, const QString &name)
{
    spec.overrides.level = name;
    return spec;
}

BRunSpec wdays(BRunSpec spec, quint64 mask) { spec.mask.wday = quint8(mask); return spec; }
BRunSpec mdays(BRunSpec spec, quint64 mask) { spec.mask.mday = quint32(mask); return spec; }
BRunSpec months(BRunSpec spec, quint64 mask) { spec.mask.month = quint16(mask); return spec; }
BRunSpec woms(BRunSpec spec, quint64 mask) { spec.mask.wom = quint8(mask); return spec; }
BRunSpec woys(BRunSpec spec, quint64 mask) { spec.mask.woy = mask; return spec; }

QString describe(const BRunSpec &s)
{
    const BRunOverrides &o = s.overrides;
    auto text = [](const std::optional<QString> &v) { return v ? '"' + *v + '"' : QString("-"); };
    auto flag = [](const std::optional<bool> &v) { return v ? QString(*v ? "yes" : "no") : QString("-"); };
    return QString("level=%1 pool=%2 full=%3 inc=%4 diff=%5 next=%6 storage=%7 messages=%8 "
                   "maxrun=%9 priority=%10 spool=%11 accurate=%12 | hour=%13 mday=%14 "
                   "month=%15 wday=%16 wom=%17 woy=%18 last=%19 minute=%20")
        .arg(text(o.level), text(o.pool), text(o.fullPool), text(o.incrementalPool),
             text(o.differentialPool), text(o.nextPool), text(o.storage), text(o.messages),
             text(o.maxRunSchedTime))
        .arg(o.priority ? QString::number(*o.priority) : QString("-"))
        .arg(flag(o.spoolData), flag(o.accurate))
        .arg(quint64(s.mask.hour), 0, 16).arg(quint64(s.mask.mday), 0, 16)
        .arg(quint64(s.mask.month), 0, 16).arg(quint64(s.mask.wday), 0, 2)
        .arg(quint64(s.mask.wom), 0, 2).arg(s.mask.woy, 0, 16)
        .arg(int(s.mask.last7DaysOfMonth)).arg(s.minute);
}

} // namespace

#define COMPARE_SPEC(actual, expected) \
    QVERIFY2((actual) == (expected), \
             qPrintable(QString("\n   actual: %1\n expected: %2") \
                            .arg(describe(actual), describe(expected))))

class BRunParserTest : public QObject
{
    Q_OBJECT

private slots:
    void defaults();
    void validLines_data();
    void validLines();
    void roundTrip_data() { validLines_data(); }
    void roundTrip();
    void untouchedLineIsByteIdentical_data() { validLines_data(); }
    void untouchedLineIsByteIdentical();
    void notUnderstood_data();
    void notUnderstood();
    void poolNamedLikeAKeyword();
    void noLevelStaysNoLevel();
    void haveFlags();
    void commentsAreNotPartOfTheRun();
    void spellingsTheDirectorPrints();
    void writerPutsOverridesFirst();
    void writerCanonicalForms_data();
    void writerCanonicalForms();
    void writerRefusesWhatNoRunLineSays();
    void touchedLineIsGeneratedAndChecked();
    void mismatchStopsTheWrite();
    void scheduleWithALineNotUnderstoodIsNotWritten();
    void weekOfMonthText();
    void quotedForCommand();
};

void BRunParserTest::defaults()
{
    // "The schedule is initialized to have the bits of each of these masks
    // set" (documentation, Technical Notes on Schedules)
    const BDateTimeMask all = BDateTimeMask::all();
    QCOMPARE(all.hour, quint32(0x00ffffff));
    QCOMPARE(all.mday, quint32(0x7fffffff));
    QCOMPARE(all.month, quint16(0x0fff));
    QCOMPARE(all.wday, quint8(0x7f));
    QCOMPARE(all.wom, quint8(0x1f));
    QCOMPARE(all.woy, Q_UINT64_C(0x3fffffffffffff));
    QVERIFY(!all.last7DaysOfMonth);

    const BRunSpec spec;
    QVERIFY(spec.mask == all);
    QCOMPARE(spec.minute, 0);
    QVERIFY(spec.overrides.isEmpty());
}

void BRunParserTest::validLines_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<BRunSpec>("expected");

    const BRunSpec everyHour;   // all masks set, minute 0

    // --- Bareos documentation, Schedule resource: WeeklyCycle ---
    QTest::newRow("doc WeeklyCycle full")
        << "Level=Full sun at 2:05" << level(wdays(at(2, 5), bits({Sun})), "Full");
    QTest::newRow("doc WeeklyCycle incremental")
        << "Level=Incremental mon-sat at 2:05"
        << level(wdays(at(2, 5), bitRange(Mon, Sat)), "Incremental");

    // --- Documentation: MonthlyCycle ---
    {
        BRunSpec s = level(woms(wdays(at(2, 5), bits({Sun})), bits({0})), "Full");
        s.overrides.pool = "Monthly";
        QTest::newRow("doc MonthlyCycle full") << "Level=Full Pool=Monthly 1st sun at 2:05" << s;
    }
    QTest::newRow("doc MonthlyCycle differential")
        << "Level=Differential 2nd-5th sun at 2:05"
        << level(woms(wdays(at(2, 5), bits({Sun})), bitRange(1, 4)), "Differential");
    {
        BRunSpec s = level(wdays(at(2, 5), bitRange(Mon, Sat)), "Incremental");
        s.overrides.pool = "Daily";
        QTest::newRow("doc MonthlyCycle incremental")
            << "Level=Incremental Pool=Daily mon-sat at 2:05" << s;
    }

    // --- Documentation: First ---
    QTest::newRow("doc First full")
        << "Level=Full on 1 at 2:05" << level(mdays(at(2, 5), bits({0})), "Full");
    QTest::newRow("doc First incremental")
        << "Level=Incremental on 2-31 at 2:05"
        << level(mdays(at(2, 5), bitRange(1, 30)), "Incremental");

    // --- Documentation: Last Friday ---
    {
        BRunSpec s = level(woms(wdays(at(21, 0), bits({Fri})), 0), "Full");
        s.mask.last7DaysOfMonth = true;
        QTest::newRow("doc Last Friday") << "Level=Full last fri at 21:00" << s;
    }

    // --- Documentation: TenMinutes ---
    for (int minute : {5, 15, 25, 35, 45, 55}) {
        BRunSpec s = level(everyHour, "Full");
        s.minute = minute;
        QTest::newRow(qPrintable(QString("doc TenMinutes :%1").arg(minute)))
            << QString("Level=Full hourly at 0:%1").arg(minute, 2, 10, QChar('0')) << s;
    }

    // --- Documentation: modulo ---
    // Odd days: the 1st, 3rd, ..., 31st
    QTest::newRow("doc Odd Days")
        << "1/2 at 23:10" << mdays(at(23, 10), Q_UINT64_C(0x55555555));
    // Even days: the 2nd, 4th, ..., 30th
    QTest::newRow("doc Even Days")
        << "2/2 at 23:10" << mdays(at(23, 10), Q_UINT64_C(0x2aaaaaaa));
    QTest::newRow("doc 3rd week in a 5-week-cycle")
        << "w03/w05 at 23:10"
        << woys(at(23, 10), bits({3, 8, 13, 18, 23, 28, 33, 38, 43, 48, 53}));
    {
        quint64 odd = 0;
        quint64 even = 0;
        for (int w = 1; w <= 53; w += 2) odd |= (Q_UINT64_C(1) << w);
        for (int w = 2; w <= 53; w += 2) even |= (Q_UINT64_C(1) << w);
        QTest::newRow("doc Odd Weeks") << "w01/w02 at 23:10" << woys(at(23, 10), odd);
        QTest::newRow("doc Even Weeks") << "w02/w02 at 23:10" << woys(at(23, 10), even);
    }

    // --- Failure cases of the review ---
    // The old parser lost Wednesday
    QTest::newRow("review tue-thu")
        << "Incremental tue-thu at 23:05"
        << level(wdays(at(23, 5), bits({Tue, Wed, Thu})), "Incremental");
    // Expected mask as in the Rust port's test: wday 1<<6, wom 1, hour 1<<21
    QTest::newRow("review 1st sat")
        << "Full 1st sat at 21:00"
        << level(woms(wdays(at(21, 0), bits({Sat})), bits({0})), "Full");
    QTest::newRow("review day of month")
        << "Full on 15 at 1:00" << level(mdays(at(1, 0), bits({14})), "Full");
    QTest::newRow("review day of month range")
        << "Full on 1-7 at 2:00" << level(mdays(at(2, 0), bitRange(0, 6)), "Full");
    QTest::newRow("review no level")
        << "sun at 1:00" << wdays(at(1, 0), bits({Sun}));

    // --- Wrap-around ranges ---
    // Rust port's test: "Level=Full fri-mon at 1:05pm" gives wday 0b1100011
    QTest::newRow("wrap fri-mon")
        << "Level=Full fri-mon at 1:05pm"
        << level(wdays(at(13, 5), bits({Fri, Sat, Sun, Mon})), "Full");
    QTest::newRow("wrap nov-feb")
        << "Full nov-feb at 3:00" << level(months(at(3, 0), bits({10, 11, 0, 1})), "Full");
    QTest::newRow("wrap day of month 28-3")
        << "Full on 28-3 at 3:00"
        << level(mdays(at(3, 0), bitRange(27, 30) | bitRange(0, 2)), "Full");
    QTest::newRow("wrap week of month 4th-1st")
        << "Full 4th-1st sun at 3:00"
        << level(woms(wdays(at(3, 0), bits({Sun})), bits({3, 4, 0})), "Full");
    QTest::newRow("wrap week of year w52-w01")
        << "Full w52-w01 at 3:00" << level(woys(at(3, 0), bits({52, 53, 0, 1})), "Full");

    // --- Times ---
    // Rust port's test: "Full at 12:30am" gives hour 1
    QTest::newRow("12:30am is 00:30") << "Full at 12:30am" << level(at(0, 30), "Full");
    QTest::newRow("12:15pm is 12:15") << "Full at 12:15pm" << level(at(12, 15), "Full");
    QTest::newRow("11:59PM") << "Full at 11:59PM" << level(at(23, 59), "Full");
    QTest::newRow("9:00am") << "Full at 9:00am" << level(at(9, 0), "Full");
    QTest::newRow("0:00") << "Full at 0:00" << level(at(0, 0), "Full");
    QTest::newRow("two digit hour") << "Full at 02:05" << level(at(2, 5), "Full");
    {
        BRunSpec s = level(at(1, 30), "Full");
        s.mask.hour = quint32(bits({1, 13}));
        QTest::newRow("two times, one minute") << "Full at 1:00 at 13:30" << s;
    }

    // --- Without "at": every hour (Rust port's test "Full sun") ---
    QTest::newRow("no at") << "Full sun" << level(wdays(everyHour, bits({Sun})), "Full");
    QTest::newRow("level only") << "Full" << level(everyHour, "Full");
    QTest::newRow("at without a time") << "Full sun at"
                                       << level(wdays(everyHour, bits({Sun})), "Full");
    QTest::newRow("hourly alone") << "Incremental hourly" << level(everyHour, "Incremental");

    // --- Lists ---
    // Rust port's test: mday 1 | 1<<14, month 1 | 1<<6
    QTest::newRow("lists of days and months")
        << "Full on 1, 15 jan,jul at 0:05"
        << level(months(mdays(at(0, 5), bits({0, 14})), bits({0, 6})), "Full");
    QTest::newRow("day list by space and comma")
        << "Full mon, wed fri at 1:00" << level(wdays(at(1, 0), bits({Mon, Wed, Fri})), "Full");
    QTest::newRow("long names")
        << "Full Saturday January-March at 1:00"
        << level(months(wdays(at(1, 0), bits({Sat})), bitRange(0, 2)), "Full");
    QTest::newRow("word positions")
        << "Full first third sun at 1:00"
        << level(woms(wdays(at(1, 0), bits({Sun})), bits({0, 2})), "Full");

    // --- Weeks of the year (Rust port's tests) ---
    QTest::newRow("w02-w04") << "Full w02-w04 at 2:00"
                             << level(woys(at(2, 0), bits({2, 3, 4})), "Full");
    QTest::newRow("w07") << "Full w07 at 2:00" << level(woys(at(2, 0), bits({7})), "Full");
    {
        quint64 weeks = 0;
        for (int w = 0; w <= 53; w += 2) weeks |= (Q_UINT64_C(1) << w);
        QTest::newRow("w00/w02") << "Full w00/w02 at 2:00" << level(woys(at(2, 0), weeks), "Full");
    }
    // Rust port's test: "Full 2/3" has the 2nd and the 5th among the first seven days
    QTest::newRow("2/3")
        << "Full 2/3 at 2:00"
        << level(mdays(at(2, 0), bits({1, 4, 7, 10, 13, 16, 19, 22, 25, 28})), "Full");

    // --- daily, weekly, monthly ---
    QTest::newRow("daily") << "Incremental daily at 23:00" << level(at(23, 0), "Incremental");
    QTest::newRow("weekly") << "Full weekly sat at 2:00"
                            << level(wdays(at(2, 0), bits({Sat})), "Full");
    QTest::newRow("monthly") << "Full monthly 1st sun at 2:00"
                             << level(woms(wdays(at(2, 0), bits({Sun})), bits({0})), "Full");
    QTest::newRow("daily widens a day") << "Full mon daily at 2:00" << level(at(2, 0), "Full");

    // --- last ---
    {
        BRunSpec s = level(woms(wdays(at(2, 0), bits({Sun})), bits({0})), "Full");
        s.mask.last7DaysOfMonth = true;
        QTest::newRow("1st and last") << "Full 1st last sun at 2:00" << s;
        QTest::newRow("last and 1st") << "Full last 1st sun at 2:00" << s;
    }

    // --- Overrides and levels ---
    {
        // The Rust port's test line
        BRunSpec s = level(at(1, 0), "Full");
        s.overrides.pool = "Monthly";
        s.overrides.storage = "File";
        s.overrides.messages = "Standard";
        s.overrides.spoolData = true;
        s.overrides.accurate = false;
        s.overrides.priority = 5;
        QTest::newRow("all common overrides")
            << "Pool=Monthly Storage=File Messages=Standard SpoolData=yes Accurate=no "
               "Priority=5 Full at 1:00" << s;
    }
    {
        BRunSpec s = level(at(1, 0), "VirtualFull");
        s.overrides.fullPool = "F";
        s.overrides.incrementalPool = "I";
        s.overrides.differentialPool = "D";
        s.overrides.nextPool = "N";
        s.overrides.maxRunSchedTime = "3600";
        QTest::newRow("pools per level")
            << "level=virtualfull FULLPOOL=F IncrementalPool=I differentialpool=D NextPool=N "
               "MaxRunSchedTime=3600 at 1:00" << s;
    }
    {
        BRunSpec s = at(1, 0);
        s.overrides.maxRunSchedTime = "1 hour 30 minutes";
        QTest::newRow("quoted time period") << "MaxRunSchedTime=\"1 hour 30 minutes\" at 1:00" << s;
    }
    QTest::newRow("old style level") << "incremental at 1:00" << level(at(1, 0), "Incremental");
    QTest::newRow("later level wins") << "Level=Full Incremental at 1:00"
                                      << level(at(1, 0), "Incremental");
    for (const QString &name : BRunParser::levelNames()) {
        QTest::newRow(qPrintable("level " + name))
            << QString("Level=%1 at 1:00").arg(name.toUpper()) << level(at(1, 0), name);
    }

    // --- Quoted values ---
    {
        BRunSpec s = level(wdays(at(1, 0), bits({Sat})), "Full");
        s.overrides.pool = "My Pool";
        QTest::newRow("quoted name with a space") << "Pool=\"My Pool\" Level=Full sat at 1:00" << s;
        s.overrides.pool = "a\"b\\c";
        QTest::newRow("quoted name with escapes")
            << "Pool=\"a\\\"b\\\\c\" Level=Full sat at 1:00" << s;
        s.overrides.pool = "at 2:00 # sun";
        QTest::newRow("quoted name with run words")
            << "Pool=\"at 2:00 # sun\" Level=Full sat at 1:00" << s;
        s.overrides.pool = "Monthly";
        QTest::newRow("spaces around the equals") << "Pool = Monthly Level = Full sat at 1:00" << s;
    }
}

void BRunParserTest::validLines()
{
    QFETCH(QString, text);
    QFETCH(BRunSpec, expected);

    const BRunLine line = BRunParser::parse(text);
    QVERIFY2(line.understood(), qPrintable(line.parseError));
    QVERIFY(line.parseError.isEmpty());
    QCOMPARE(line.original, text);
    QVERIFY(!line.dirty);
    COMPARE_SPEC(*line.spec, expected);
}

void BRunParserTest::roundTrip()
{
    QFETCH(QString, text);

    // write(parse(x)), parsed again, gives the same spec
    const BRunLine line = BRunParser::parse(text);
    QVERIFY(line.understood());
    QString error;
    const QString written = BRunWriter::write(*line.spec, &error);
    QVERIFY2(!written.isEmpty(), qPrintable(error));

    const BRunLine again = BRunParser::parse(written);
    QVERIFY2(again.understood(), qPrintable(written + ": " + again.parseError));
    QVERIFY2(!again.needsRewrite, qPrintable(written));
    COMPARE_SPEC(*again.spec, *line.spec);

    // What the writer wrote is its own canonical form
    QCOMPARE(BRunWriter::write(*again.spec), written);
}

void BRunParserTest::untouchedLineIsByteIdentical()
{
    QFETCH(QString, text);

    const BRunLine line = BRunParser::parse(text);
    QVERIFY(line.understood());
    QVERIFY(!line.needsRewrite);
    QString error;
    QCOMPARE(BRunWriter::lineText(line, &error), text);
    QVERIFY(error.isEmpty());
}

void BRunParserTest::notUnderstood_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("errorContains");

    // --- Failure cases of the review ---
    QTest::newRow("unknown level") << "Level=Bogus sun at 1:00" << "Bogus";
    QTest::newRow("unknown bare level") << "Bogus sun at 1:00" << "Bogus";
    // What the old wizard wrote: Bareos reads no override behind the time
    QTest::newRow("override behind the time") << "Full sat at 21:00 pool=Monthly" << "pool";
    QTest::newRow("priority behind the time") << "Level=Incremental daily at 23:00 priority=10"
                                              << "priority";

    // --- The Rust port's rejections ---
    QTest::newRow("time without at") << "Full 12:30am" << "AT";
    QTest::newRow("unknown word") << "Full nosuchday at 1:00" << "nosuchday";
    QTest::newRow("range of a day and a month") << "Full mon-jan at 1:00" << "range";

    // --- Times ---
    QTest::newRow("hour only") << "Incremental mon-fri at 9pm" << "9pm";
    QTest::newRow("one minute digit") << "Full at 2:5" << "time";
    QTest::newRow("hour 24") << "Full at 24:00" << "time";
    QTest::newRow("minute 60") << "Full at 2:60" << "time";
    QTest::newRow("13pm") << "Full at 13:00pm" << "time";
    QTest::newRow("seconds") << "Full at 2:05:00" << "time";
    QTest::newRow("letters as hour") << "Full at x:05" << "time";
    QTest::newRow("letters as minute") << "Full at 2:xx" << "time";

    // --- Ranges, lists, modulo ---
    QTest::newRow("day 0") << "Full on 0 at 1:00" << "1-31";
    QTest::newRow("day 32") << "Full on 32 at 1:00" << "1-31";
    QTest::newRow("day range to 32") << "Full on 1-32 at 1:00" << "range";
    QTest::newRow("range of one day") << "Full mon-mon at 1:00" << "range";
    QTest::newRow("open range") << "Full mon- at 1:00" << "range";
    QTest::newRow("week 54") << "Full w54 at 1:00" << "0-53";
    QTest::newRow("modulo by zero") << "Full 1/0 at 1:00" << "modulo";
    QTest::newRow("modulo offset above interval") << "Full 5/2 at 1:00" << "modulo";
    QTest::newRow("week modulo offset above interval") << "Full w10/w05 at 1:00" << "modulo";
    QTest::newRow("modulo of words") << "Full mon/tue at 1:00" << "modulo";

    // --- Overrides ---
    QTest::newRow("override without equals") << "Pool Monthly at 1:00" << "equals";
    // "at" is the pool's name here, and the time has no "at" in front of it
    QTest::newRow("override takes the next word") << "Pool= at 1:00" << "AT";
    QTest::newRow("override without value") << "Pool=, at 1:00" << "value";
    QTest::newRow("override at the end") << "Pool=" << "value";
    QTest::newRow("priority not a number") << "Priority=high at 1:00" << "integer";
    QTest::newRow("spooldata maybe") << "SpoolData=maybe at 1:00" << "YES or NO";
    QTest::newRow("bad time period") << "MaxRunSchedTime=soon at 1:00" << "time period";
    // The Director prints virtualfullpool=, the Bareos parser does not read it
    QTest::newRow("virtualfullpool") << "virtualfullpool=\"VF\" Full at 1:00" << "virtualfullpool";
    // The documentation allows a comma between overrides; run_conf.cc and
    // the Rust port read only date words after it
    QTest::newRow("comma between overrides") << "Level=Full, Pool=Monthly sun at 2:05" << "Pool";
    QTest::newRow("stray equals") << "Full sun = at 1:00" << "=";

    // --- Lexer ---
    QTest::newRow("empty") << "" << "empty";
    QTest::newRow("blank") << "   " << "empty";
    QTest::newRow("comment only") << "# Full sun at 1:00" << "empty";
    QTest::newRow("open quote") << "Pool=\"Monthly Full at 1:00" << "quoted";
    QTest::newRow("quoted date word") << "Full \"sun\" at 1:00" << "sun";
    QTest::newRow("line break") << "Full sun at 1:00\nFull mon at 1:00" << "line break";
    QTest::newRow("brace") << "Full sun at 1:00 }" << "}";
}

void BRunParserTest::notUnderstood()
{
    QFETCH(QString, text);
    QFETCH(QString, errorContains);

    const BRunLine line = BRunParser::parse(text);
    QVERIFY2(!line.understood(), qPrintable(text));
    QVERIFY(!line.spec.has_value());
    // The text is kept verbatim, with the reason
    QCOMPARE(line.original, text);
    QVERIFY2(line.parseError.contains(errorContains),
             qPrintable(QString("\"%1\" does not contain \"%2\"").arg(line.parseError, errorContains)));
    QVERIFY(!line.needsRewrite);

    // Such a line is never rewritten
    QString error;
    QVERIFY(BRunWriter::lineText(line, &error).isEmpty());
    QVERIFY(!error.isEmpty());
}

void BRunParserTest::poolNamedLikeAKeyword()
{
    // The old parser read "monthly" and "daily" out of the pool's name and
    // made the run happen every day
    BRunLine line = BRunParser::parse("Level=Full Pool=Monthly 1st sun at 2:05");
    QVERIFY(line.understood());
    QCOMPARE(line.spec->overrides.pool, std::optional<QString>("Monthly"));
    QCOMPARE(line.spec->mask.month, BDateTimeMask::AllMonths);
    QCOMPARE(line.spec->mask.wday, quint8(1));
    QVERIFY(!line.spec->haveMonth);

    line = BRunParser::parse("Level=Incremental Pool=Daily mon-sat at 2:05");
    QVERIFY(line.understood());
    QCOMPARE(line.spec->overrides.pool, std::optional<QString>("Daily"));
    QCOMPARE(line.spec->mask.wday, quint8(0x7e));

    // Names of levels, days and hours as values are values
    line = BRunParser::parse("Pool=Full Storage=sun Messages=hourly Level=Incremental sat at 1:00");
    QVERIFY(line.understood());
    QCOMPARE(line.spec->overrides.level, std::optional<QString>("Incremental"));
    QCOMPARE(line.spec->overrides.pool, std::optional<QString>("Full"));
    QCOMPARE(line.spec->overrides.storage, std::optional<QString>("sun"));
    QCOMPARE(line.spec->overrides.messages, std::optional<QString>("hourly"));
    QCOMPARE(line.spec->mask.wday, quint8(1 << Sat));
    QCOMPARE(line.spec->mask.hour, quint32(1 << 1));
}

void BRunParserTest::noLevelStaysNoLevel()
{
    const BRunLine line = BRunParser::parse("sun at 1:00");
    QVERIFY(line.understood());
    QVERIFY(!line.spec->overrides.level.has_value());
    QVERIFY(line.spec->overrides.isEmpty());

    // And the writer does not invent one
    const QString written = BRunWriter::write(*line.spec);
    QCOMPARE(written, QString("sun at 01:00"));
    QVERIFY(!BRunParser::parse(written).spec->overrides.level.has_value());

    QCOMPARE(BRunParser::canonicalLevel("FULL"), QString("Full"));
    QCOMPARE(BRunParser::canonicalLevel("virtualfull"), QString("VirtualFull"));
    QCOMPARE(BRunParser::canonicalLevel("Bogus"), QString());
    QCOMPARE(BRunParser::canonicalLevel(""), QString());
}

void BRunParserTest::haveFlags()
{
    BRunLine line = BRunParser::parse("Full sun");
    QVERIFY(line.spec->haveWday);
    QVERIFY(!line.spec->haveAt);
    QVERIFY(!line.spec->haveHour);
    QVERIFY(!line.spec->haveMday);
    QVERIFY(!line.spec->haveMonth);
    QVERIFY(!line.spec->haveWom);
    QVERIFY(!line.spec->haveWoy);

    line = BRunParser::parse("Full jan on 1 1st w01 at 2:05");
    QVERIFY(line.spec->haveAt);
    QVERIFY(line.spec->haveHour);
    QVERIFY(line.spec->haveMday);
    QVERIFY(line.spec->haveMonth);
    QVERIFY(line.spec->haveWom);
    QVERIFY(line.spec->haveWoy);
    QVERIFY(!line.spec->haveWday);
    QCOMPARE(line.spec->hours(), QList<int>({2}));

    // The flags say how a line was written; two lines that run at the same
    // times are the same run
    QVERIFY(*BRunParser::parse("Full at 2:00").spec == *BRunParser::parse("Full daily at 2:00").spec);
    QVERIFY(*BRunParser::parse("Full sun-sat at 2:00").spec == *BRunParser::parse("Full at 02:00").spec);
    QVERIFY(*BRunParser::parse("Full tue-thu at 2:00").spec
            == *BRunParser::parse("FULL  Thu,tue Wednesday  at 2:00am").spec);
    QVERIFY(*BRunParser::parse("Full tue-thu at 2:00").spec != *BRunParser::parse("Full tue,thu at 2:00").spec);
    QVERIFY(*BRunParser::parse("Full sat at 2:00").spec != *BRunParser::parse("sat at 2:00").spec);
    QVERIFY(*BRunParser::parse("Full sat at 2:00").spec != *BRunParser::parse("Full sat at 2:01").spec);
}

void BRunParserTest::commentsAreNotPartOfTheRun()
{
    const BRunSpec plain = *BRunParser::parse("Level=Full sun at 2:05").spec;

    for (const QString &text : {QString("Level=Full sun at 2:05 # every week"),
                                QString("Level=Full sun at 2:05#x"),
                                QString("Level=Full sun at 2:05 ; Level=Incremental"),
                                QString("  Level=Full   sun\tat 2:05  ")}) {
        const BRunLine line = BRunParser::parse(text);
        QVERIFY2(line.understood(), qPrintable(text));
        COMPARE_SPEC(*line.spec, plain);
        // Untouched, the comment and the spacing stay
        QCOMPARE(BRunWriter::lineText(line), text);
    }
}

void BRunParserTest::spellingsTheDirectorPrints()
{
    // dird_conf.cc PrintConfigRun(): quoted names, the level as a bare
    // word, capitals, two-digit hours
    BRunLine line = BRunParser::parse("pool=\"Monthly\" Full 1st Sun at 02:05");
    QVERIFY(line.understood());
    QVERIFY(!line.needsRewrite);
    COMPARE_SPEC(*line.spec, *BRunParser::parse("Level=Full Pool=Monthly 1st sun at 2:05").spec);
    QCOMPARE(BRunWriter::lineText(line), QString("pool=\"Monthly\" Full 1st Sun at 02:05"));

    line = BRunParser::parse("Incremental Mon-Sat at 02:05");
    QVERIFY(line.understood());
    QVERIFY(!line.needsRewrite);

    // A day of the month directly behind the overrides is printed, but
    // Bareos reads it back only as "on 1". Understood, and respelled on write.
    line = BRunParser::parse("Full 1 at 02:05");
    QVERIFY(line.understood());
    QVERIFY(line.needsRewrite);
    COMPARE_SPEC(*line.spec, *BRunParser::parse("Level=Full on 1 at 2:05").spec);
    QCOMPARE(BRunWriter::lineText(line), QString("Level=Full on 1 at 02:05"));

    line = BRunParser::parse("2-31 at 02:05");
    QVERIFY(line.understood());
    QVERIFY(!line.needsRewrite);    // a range is a word, not a number

    // Several hours are printed glued together
    QString glued = "Full ";
    for (int h = 0; h < 24; ++h)
        glued += QString("at %1:05").arg(h, 2, 10, QChar('0'));
    line = BRunParser::parse(glued);
    QVERIFY2(line.understood(), qPrintable(line.parseError));
    QVERIFY(line.needsRewrite);
    COMPARE_SPEC(*line.spec, *BRunParser::parse("Level=Full hourly at 0:05").spec);
    QCOMPARE(BRunWriter::lineText(line), QString("Level=Full hourly at 0:05"));

    line = BRunParser::parse("Full Sat at 01:30at 13:30");
    QVERIFY(line.understood());
    QVERIFY(line.needsRewrite);
    QCOMPARE(line.spec->hours(), QList<int>({1, 13}));
    QCOMPARE(BRunWriter::lineText(line), QString("Level=Full sat at 01:30 at 13:30"));
}

void BRunParserTest::writerPutsOverridesFirst()
{
    BRunSpec spec = at(2, 5);
    spec.mask.wday = quint8(bits({Sun}));
    spec.mask.wom = quint8(bits({0}));
    spec.overrides.level = "Full";
    spec.overrides.pool = "Monthly";
    spec.overrides.fullPool = "F";
    spec.overrides.incrementalPool = "I";
    spec.overrides.differentialPool = "D";
    spec.overrides.nextPool = "N";
    spec.overrides.storage = "File";
    spec.overrides.messages = "Standard";
    spec.overrides.priority = 5;
    spec.overrides.spoolData = true;
    spec.overrides.accurate = false;
    spec.overrides.maxRunSchedTime = "2 hours";

    const QString written = BRunWriter::write(spec);
    QCOMPARE(written,
             QString("Level=Full Pool=Monthly FullPool=F IncrementalPool=I DifferentialPool=D "
                     "NextPool=N Storage=File Messages=Standard Priority=5 SpoolData=yes "
                     "Accurate=no MaxRunSchedTime=\"2 hours\" 1st sun at 02:05"));

    // Every override stands in front of the first date or time word: Bareos
    // reads nothing but date words after them
    const int firstDateWord = written.indexOf(" 1st ");
    QVERIFY(firstDateWord > 0);
    QVERIFY(written.lastIndexOf('=') < firstDateWord);
    QVERIFY(written.indexOf(" at ") > firstDateWord);

    const BRunLine again = BRunParser::parse(written);
    QVERIFY2(again.understood(), qPrintable(again.parseError));
    COMPARE_SPEC(*again.spec, spec);

    // What the old wizard produced for the same run is not a Run line
    QVERIFY(!BRunParser::parse("Full 1st sun at 02:05 pool=Monthly storage=File priority=5").understood());
}

void BRunParserTest::writerCanonicalForms_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("canonical");

    QTest::newRow("level=") << "Full sat at 2:05" << "Level=Full sat at 02:05";
    QTest::newRow("no level") << "sat at 2:05" << "sat at 02:05";
    QTest::newRow("every day") << "Full daily at 2:05" << "Level=Full at 02:05";
    QTest::newRow("day range") << "Incremental mon-fri at 23:05" << "Level=Incremental mon-fri at 23:05";
    QTest::newRow("two days") << "Full sat,sun at 1:00" << "Level=Full sun,sat at 01:00";
    QTest::newRow("wrapping range in ascending order")
        << "Full fri-mon at 1:00" << "Level=Full sun,mon,fri,sat at 01:00";
    QTest::newRow("months") << "Full nov-feb at 1:00" << "Level=Full jan,feb,nov,dec at 01:00";
    QTest::newRow("day of month has on") << "Full 15 at 1:00" << "Level=Full on 15 at 01:00";
    QTest::newRow("day range of month") << "Full on 2-31 at 1:00" << "Level=Full on 2-31 at 01:00";
    QTest::newRow("day modulo") << "1/2 at 23:10" << "on 1/2 at 23:10";
    QTest::newRow("even days") << "2/2 at 23:10" << "on 2/2 at 23:10";
    QTest::newRow("week modulo") << "w03/w05 at 23:10" << "w03/w05 at 23:10";
    QTest::newRow("week range") << "Full w02-w04 at 2:00" << "Level=Full w02-w04 at 02:00";
    QTest::newRow("last") << "Full last fri at 21:00" << "Level=Full last fri at 21:00";
    QTest::newRow("1st and last") << "Full last 1st fri at 21:00" << "Level=Full 1st last fri at 21:00";
    QTest::newRow("hourly") << "Full hourly at 0:15" << "Level=Full hourly at 0:15";
    QTest::newRow("no time") << "Full sun" << "Level=Full sun hourly at 0:00";
    QTest::newRow("several hours") << "Full at 1:30 at 13:30" << "Level=Full at 01:30 at 13:30";
    QTest::newRow("pm") << "Full at 1:05pm" << "Level=Full at 13:05";
    QTest::newRow("order of the parts")
        << "Full on 1 sun 1st w01 jan at 2:00" << "Level=Full jan w01 1st sun on 1 at 02:00";
    QTest::newRow("quoted name") << "Pool=\"My Pool\" Full at 1:00"
                                 << "Level=Full Pool=\"My Pool\" at 01:00";
    QTest::newRow("name with a dash") << "Pool=\"Full-Pool\" Full at 1:00"
                                      << "Level=Full Pool=Full-Pool at 01:00";
    QTest::newRow("name of digits stays quoted") << "Pool=\"2024\" Full at 1:00"
                                                 << "Level=Full Pool=\"2024\" at 01:00";
}

void BRunParserTest::writerCanonicalForms()
{
    QFETCH(QString, text);
    QFETCH(QString, canonical);

    const BRunLine line = BRunParser::parse(text);
    QVERIFY2(line.understood(), qPrintable(line.parseError));
    QCOMPARE(BRunWriter::write(*line.spec), canonical);

    const BRunLine again = BRunParser::parse(canonical);
    QVERIFY2(again.understood(), qPrintable(again.parseError));
    QVERIFY(!again.needsRewrite);
    COMPARE_SPEC(*again.spec, *line.spec);
}

void BRunParserTest::writerRefusesWhatNoRunLineSays()
{
    QString error;

    BRunSpec spec = at(2, 5);
    spec.mask.wday = 0;
    QVERIFY(BRunWriter::write(spec, &error).isEmpty());
    QVERIFY(!error.isEmpty());

    spec = at(2, 5);
    spec.mask.hour = 0;
    QVERIFY(BRunWriter::write(spec, &error).isEmpty());

    // No week of the month at all, and not "last" either
    spec = at(2, 5);
    spec.mask.wom = 0;
    QVERIFY(BRunWriter::write(spec, &error).isEmpty());
    spec.mask.last7DaysOfMonth = true;
    QCOMPARE(BRunWriter::write(spec, &error), QString("last at 02:05"));
    QVERIFY(error.isEmpty());

    spec = at(2, 5);
    spec.minute = 60;
    QVERIFY(BRunWriter::write(spec, &error).isEmpty());
    QVERIFY(!error.isEmpty());
}

void BRunParserTest::touchedLineIsGeneratedAndChecked()
{
    BRunLine line = BRunParser::parse("pool=\"Monthly\" Full 1st Sun at 02:05 # first sunday");
    QVERIFY(line.understood());

    // Moved by an hour: generated from the spec, overrides first
    line.spec->mask.hour = quint32(1u << 3);
    line.spec->minute = 15;
    line.dirty = true;

    QString error;
    const QString text = BRunWriter::lineText(line, &error);
    QCOMPARE(text, QString("Level=Full Pool=Monthly 1st sun at 03:15"));
    QVERIFY(error.isEmpty());
    COMPARE_SPEC(*BRunParser::parse(text).spec, *line.spec);

    // A new line has no original text and is generated as well
    BRunLine fresh;
    fresh.spec = level(wdays(at(23, 5), bitRange(Mon, Fri)), "Incremental");
    QCOMPARE(BRunWriter::lineText(fresh), QString("Level=Incremental mon-fri at 23:05"));
}

void BRunParserTest::mismatchStopsTheWrite()
{
    // A name with a line break cannot be said on a Run line. The writer
    // writes it, the check reads it back, does not get the same and stops.
    BRunLine line;
    line.spec = level(at(1, 0), "Full");
    line.spec->overrides.pool = "Monthly\nFull";
    line.dirty = true;

    QVERIFY(!BRunWriter::write(*line.spec).isEmpty());
    QString error;
    QVERIFY(BRunWriter::lineText(line, &error).isEmpty());
    QVERIFY(!error.isEmpty());

    // A level Bareos does not know reads back as not understood
    line.spec = level(at(1, 0), "Bogus");
    QVERIFY(BRunWriter::lineText(line, &error).isEmpty());
    QVERIFY2(error.contains("Bogus"), qPrintable(error));

    // An empty pool name is written as "" and does not read back
    line.spec = level(at(1, 0), "Full");
    line.spec->overrides.pool = QString("");
    QVERIFY(BRunWriter::lineText(line, &error).isEmpty());
    QVERIFY(!error.isEmpty());
}

void BRunParserTest::scheduleWithALineNotUnderstoodIsNotWritten()
{
    BSchedule schedule;
    schedule.name = "MonthlyCycle";
    for (const QString &text : {QString("Level=Full Pool=Monthly 1st sun at 2:05"),
                                QString("Level=Differential 2nd-5th sun at 2:05"),
                                QString("Level=Incremental Pool=Daily mon-sat at 2:05")}) {
        schedule.runs.append(BRunParser::parse(text));
    }

    // All understood and untouched: the lines as they were
    QStringList texts;
    QString error;
    QVERIFY(BRunWriter::runTexts(schedule.runs, &texts, &error));
    QCOMPARE(texts, QStringList({"Level=Full Pool=Monthly 1st sun at 2:05",
                                 "Level=Differential 2nd-5th sun at 2:05",
                                 "Level=Incremental Pool=Daily mon-sat at 2:05"}));
    QVERIFY(error.isEmpty());

    // One touched line: only that one is generated
    schedule.runs[1].spec->mask.hour = quint32(1u << 4);
    schedule.runs[1].dirty = true;
    QVERIFY(BRunWriter::runTexts(schedule.runs, &texts, &error));
    QCOMPARE(texts, QStringList({"Level=Full Pool=Monthly 1st sun at 2:05",
                                 "Level=Differential 2nd-5th sun at 04:05",
                                 "Level=Incremental Pool=Daily mon-sat at 2:05"}));

    // One line that is not understood: nothing is written, and the error
    // names the line and the reason
    schedule.runs.append(BRunParser::parse("Level=Full every full moon at 0:00"));
    QVERIFY(!BRunWriter::runTexts(schedule.runs, &texts, &error));
    QVERIFY(texts.isEmpty());
    QVERIFY2(error.contains("4"), qPrintable(error));
    QVERIFY2(error.contains("every full moon"), qPrintable(error));
    QVERIFY2(error.contains("every in run record not found"), qPrintable(error));

    // The same from the texts alone, as the configuration export uses it
    const QStringList reported = {"Full 1st Sat at 21:00", "Full 1 at 02:05", "sun at 1:00"};
    QVERIFY(BRunWriter::runTexts(reported, &texts, &error));
    QCOMPARE(texts, QStringList({"Full 1st Sat at 21:00", "Level=Full on 1 at 02:05", "sun at 1:00"}));
    QVERIFY(!BRunWriter::runTexts(QStringList({"Full sat at 21:00", "Full sat at 21:00 pool=Monthly"}),
                                  &texts, &error));
    QVERIFY(texts.isEmpty());
    QVERIFY2(error.contains("Run line 2"), qPrintable(error));
}

void BRunParserTest::weekOfMonthText()
{
    BDateTimeMask mask = BDateTimeMask::all();
    QCOMPARE(BRunWriter::weekOfMonthText(mask), QString());
    mask.wom = quint8(bits({0}));
    QCOMPARE(BRunWriter::weekOfMonthText(mask), QString("1st"));
    mask.wom = quint8(bitRange(1, 4));
    QCOMPARE(BRunWriter::weekOfMonthText(mask), QString("2nd-5th"));
    mask.wom = quint8(bits({0, 2}));
    QCOMPARE(BRunWriter::weekOfMonthText(mask), QString("1st,3rd"));
    mask.wom = 0;
    mask.last7DaysOfMonth = true;
    QCOMPARE(BRunWriter::weekOfMonthText(mask), QString("last"));
    // "last" clears the weeks unless they were named: with all of them set
    // they have to be named
    mask.wom = BDateTimeMask::AllWoms;
    QCOMPARE(BRunWriter::weekOfMonthText(mask), QString("1st-5th last"));
    const BRunLine line = BRunParser::parse("1st-5th last at 1:00");
    QCOMPARE(line.spec->mask.wom, BDateTimeMask::AllWoms);
    QVERIFY(line.spec->mask.last7DaysOfMonth);
}

void BRunParserTest::quotedForCommand()
{
    QCOMPARE(BRunWriter::quotedForCommand("Level=Full sun at 02:05"),
             QString("\"Level=Full sun at 02:05\""));
    // The Director's command line takes \" as a quote inside a quoted value
    QCOMPARE(BRunWriter::quotedForCommand("pool=\"My Pool\" Full sun at 02:05"),
             QString("\"pool=\\\"My Pool\\\" Full sun at 02:05\""));
    QCOMPARE(BRunWriter::quotedForCommand("Pool=\"a\\\\b\" at 1:00"),
             QString("\"Pool=\\\"a\\\\\\\\b\\\" at 1:00\""));
}

QTEST_APPLESS_MAIN(BRunParserTest)
#include "brunparser_test.moc"
