/**
 * @file bstatuschip_test.cpp
 * @brief Status chips: tone, code and words; theme tokens; the delegate
 *
 * - Every Bareos 25 JobStatus letter (core/src/include/job_status.h) has a
 *   tone, a code, a short word and a full word. Only 'T' is green.
 * - Client states: a recent backup is not green; codes differ per state.
 * - Volume states get the tones of their meaning.
 * - Both themes define every token, BStatusPalette reads them when the
 *   theme is applied, and they equal the built-in values; text on its fill
 *   has a contrast of at least 4.5:1 (WCAG AA).
 * - BStatusDelegate paints the fill of the tone, keeps the selection
 *   visible around the chip and falls back to a plain cell without a tone.
 *
 * Widgets on the offscreen platform; settings in a temporary directory.
 */

#include <QtTest>
#include <QApplication>
#include <QFile>
#include <QImage>
#include <QPainter>
#include <QSet>
#include <QSettings>
#include <QStandardItemModel>
#include <QStyleFactory>
#include <QTemporaryDir>

#include "bstatuschip.h"
#include "bstatusdelegate.h"
#include "buiscale.h"

using BStatus::Part;
using BStatus::Tone;

Q_DECLARE_METATYPE(BStatus::Tone)

namespace {

const Tone kTones[] = {Tone::Neutral, Tone::Success, Tone::Warning, Tone::Error,
                       Tone::Running, Tone::Waiting, Tone::Info};
const Part kParts[] = {Part::Background, Part::Foreground, Part::Accent};

QString theme(const char *name)
{
    QFile file(QStringLiteral(THEME_DIR "/%1.qss").arg(QLatin1String(name)));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return QString();
    return QString::fromUtf8(file.readAll());
}

double luminance(const QColor &c)
{
    auto channel = [](double v) {
        v /= 255.0;
        return v <= 0.03928 ? v / 12.92 : std::pow((v + 0.055) / 1.055, 2.4);
    };
    return 0.2126 * channel(c.red()) + 0.7152 * channel(c.green()) + 0.0722 * channel(c.blue());
}

double contrast(const QColor &a, const QColor &b)
{
    const double la = luminance(a);
    const double lb = luminance(b);
    return (qMax(la, lb) + 0.05) / (qMin(la, lb) + 0.05);
}

} // namespace

class BStatusChipTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanup();

    void jobStatus_data();
    void jobStatus();
    void onlyTerminatedIsGreen();
    void unknownJobStatus();
    void jobErrors();
    void clientStates();
    void volumeTones_data();
    void volumeTones();

    void themeTokens_data();
    void themeTokens();
    void tokensAreReadable();

    void delegatePaintsTheTone();
    void delegateKeepsTheSelection();
    void delegateExplicitFill();
    void delegateWithoutTone();

private:
    QTemporaryDir m_config;
};

void BStatusChipTest::initTestCase()
{
    QVERIFY(m_config.isValid());
    qputenv("XDG_CONFIG_HOME", m_config.path().toUtf8());
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, m_config.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_config.path());
    QApplication::setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
    BUiScale::instance().setBaseFont(QFont(QStringLiteral("DejaVu Sans"), 10));
}

void BStatusChipTest::cleanup()
{
    qApp->setStyleSheet(QString());
    BStatus::resetColors();
}

// --- Job status ----------------------------------------------------------------------

void BStatusChipTest::jobStatus_data()
{
    QTest::addColumn<QString>("letter");
    QTest::addColumn<Tone>("tone");
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("full");

    QTest::newRow("T") << "T" << Tone::Success << "OK" << "Terminated normally";
    QTest::newRow("W") << "W" << Tone::Warning << "Warnings" << "Terminated with warnings";
    QTest::newRow("E") << "E" << Tone::Error << "Error" << "Terminated in error";
    QTest::newRow("e") << "e" << Tone::Error << "Errors" << "Non-fatal error";
    QTest::newRow("f") << "f" << Tone::Error << "Fatal" << "Fatal error";
    QTest::newRow("A") << "A" << Tone::Neutral << "Canceled" << "Canceled by user";
    QTest::newRow("R") << "R" << Tone::Running << "Running" << "Running";
    QTest::newRow("L") << "L" << Tone::Running << "Committing" << "Committing data (last despool)";
    QTest::newRow("l") << "l" << Tone::Running << "Despooling" << "Despooling data";
    QTest::newRow("C") << "C" << Tone::Waiting << "Created" << "Created, not yet running";
    QTest::newRow("F") << "F" << Tone::Waiting << "Wait FD" << "Waiting on the File daemon";
    QTest::newRow("S") << "S" << Tone::Waiting << "Wait SD" << "Waiting on the Storage daemon";
    QTest::newRow("m") << "m" << Tone::Waiting << "Wait media" << "Waiting for new media";
    QTest::newRow("M") << "M" << Tone::Waiting << "Wait mount" << "Waiting for mount";
    QTest::newRow("s") << "s" << Tone::Waiting << "Wait storage" << "Waiting for a storage resource";
    QTest::newRow("j") << "j" << Tone::Waiting << "Wait job" << "Waiting for a job resource";
    QTest::newRow("c") << "c" << Tone::Waiting << "Wait client" << "Waiting for a client resource";
    QTest::newRow("d") << "d" << Tone::Waiting << "Wait max jobs" << "Waiting for maximum jobs";
    QTest::newRow("t") << "t" << Tone::Waiting << "Wait start" << "Waiting for the start time";
    QTest::newRow("p") << "p" << Tone::Waiting << "Wait priority" << "Waiting for higher priority jobs to finish";
    QTest::newRow("q") << "q" << Tone::Waiting << "Wait device" << "Queued, waiting for a device";
    QTest::newRow("a") << "a" << Tone::Waiting << "Despool attr." << "Storage daemon despooling attributes";
    QTest::newRow("i") << "i" << Tone::Waiting << "Inserting" << "Doing batch insert of file records";
    QTest::newRow("B") << "B" << Tone::Warning << "Blocked" << "Blocked";
    QTest::newRow("D") << "D" << Tone::Warning << "Differences" << "Verify found differences";
    QTest::newRow("I") << "I" << Tone::Warning << "Incomplete" << "Incomplete job";
}

void BStatusChipTest::jobStatus()
{
    QFETCH(QString, letter);
    QFETCH(Tone, tone);
    QFETCH(QString, text);
    QFETCH(QString, full);

    const BStatus::Chip chip = BStatus::jobStatus(letter);
    QCOMPARE(chip.tone, tone);
    QCOMPARE(chip.code, letter);
    QCOMPARE(chip.text, text);
    QCOMPARE(chip.fullText, full);
    QCOMPARE(BStatus::jobStatusText(letter), full);
}

void BStatusChipTest::onlyTerminatedIsGreen()
{
    // Every letter of job_status.h; codes are unique, so colour is never
    // the only difference between two states
    const QString letters = QStringLiteral("ABCDEFILMRSTWacdefijlmpqst");
    QSet<QString> codes;
    for (const QChar c : letters) {
        const BStatus::Chip chip = BStatus::jobStatus(QString(c));
        QVERIFY2(!chip.text.isEmpty() && !chip.fullText.isEmpty(), qPrintable(QString(c)));
        QCOMPARE(chip.tone == Tone::Success, c == QLatin1Char('T'));
        codes.insert(chip.code);
    }
    QCOMPARE(codes.size(), letters.size());
}

void BStatusChipTest::unknownJobStatus()
{
    const BStatus::Chip x = BStatus::jobStatus(QStringLiteral("X"));
    QCOMPARE(x.tone, Tone::Neutral);
    QCOMPARE(x.code, QStringLiteral("X"));
    QCOMPARE(x.fullText, QStringLiteral("Status X"));

    const BStatus::Chip empty = BStatus::jobStatus(QString());
    QCOMPARE(empty.tone, Tone::Neutral);
    QCOMPARE(empty.code, QStringLiteral("?"));
    QCOMPARE(empty.text, QStringLiteral("Unknown"));
}

void BStatusChipTest::jobErrors()
{
    QVERIFY(BStatus::isJobError(QStringLiteral("E")));
    QVERIFY(BStatus::isJobError(QStringLiteral("e")));
    QVERIFY(BStatus::isJobError(QStringLiteral("f")));
    // 'F' is "waiting on the File daemon", not a failure
    QVERIFY(!BStatus::isJobError(QStringLiteral("F")));
    QVERIFY(!BStatus::isJobError(QStringLiteral("T")));
    QVERIFY(!BStatus::isJobError(QStringLiteral("A")));
}

// --- Clients and volumes ---------------------------------------------------------------

void BStatusChipTest::clientStates()
{
    using BClientStatus::Activity;
    const BStatus::Chip recent = BStatus::clientActivity(Activity::Recent, QStringLiteral("3 h ago"));
    const BStatus::Chip stale = BStatus::clientActivity(Activity::Stale, QStringLiteral("5 d ago"));
    const BStatus::Chip never = BStatus::clientActivity(Activity::Never, QStringLiteral("Never"));
    const BStatus::Chip unknown = BStatus::clientActivity(Activity::Unknown, QStringLiteral("Unknown"));

    // A recent backup or contact is a fact about the past, not a verified
    // "online": no green for any client state
    QCOMPARE(recent.tone, Tone::Info);
    QCOMPARE(stale.tone, Tone::Warning);
    QCOMPARE(never.tone, Tone::Neutral);
    QCOMPARE(unknown.tone, Tone::Neutral);

    QCOMPARE(recent.text, QStringLiteral("3 h ago"));
    QCOMPARE(recent.fullText, QStringLiteral("Recent, 3 h ago"));
    QCOMPARE(stale.fullText, QStringLiteral("Stale, 5 d ago"));
    QCOMPARE(never.fullText, QStringLiteral("Never"));
    QCOMPARE(unknown.fullText, QStringLiteral("Unknown"));

    const QSet<QString> codes{recent.code, stale.code, never.code, unknown.code};
    QCOMPARE(codes.size(), 4);
}

void BStatusChipTest::volumeTones_data()
{
    QTest::addColumn<QString>("status");
    QTest::addColumn<Tone>("tone");
    QTest::newRow("Append") << "Append" << Tone::Info;
    QTest::newRow("Full") << "Full" << Tone::Neutral;
    QTest::newRow("Used") << "Used" << Tone::Neutral;
    QTest::newRow("Purged") << "Purged" << Tone::Waiting;
    QTest::newRow("Recycle") << "Recycle" << Tone::Waiting;
    QTest::newRow("Error") << "Error" << Tone::Error;
    QTest::newRow("Read-Only") << "Read-Only" << Tone::Neutral;
    QTest::newRow("Disabled") << "Disabled" << Tone::Neutral;
    QTest::newRow("Archive") << "Archive" << Tone::Neutral;
    QTest::newRow("Busy") << "Busy" << Tone::Running;
    QTest::newRow("Cleaning") << "Cleaning" << Tone::Waiting;
    QTest::newRow("Unlabeled") << "Unlabeled" << Tone::Warning;
    QTest::newRow("empty") << "" << Tone::Neutral;
}

void BStatusChipTest::volumeTones()
{
    QFETCH(QString, status);
    QFETCH(Tone, tone);
    QCOMPARE(BStatus::volumeTone(status), tone);
}

// --- Theme tokens ------------------------------------------------------------------------

void BStatusChipTest::themeTokens_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<bool>("dark");
    QTest::newRow("dark") << "dark" << true;
    QTest::newRow("light") << "light" << false;
}

void BStatusChipTest::themeTokens()
{
    QFETCH(QString, name);
    QFETCH(bool, dark);

    const QString sheet = theme(qPrintable(name));
    QVERIFY(!sheet.isEmpty());
    for (const Tone tone : kTones) {
        for (const char *part : {"Background", "Foreground", "Accent"}) {
            const QString property = QStringLiteral("qproperty-%1%2:").arg(BStatus::toneName(tone),
                                                                          QLatin1String(part));
            QVERIFY2(sheet.contains(property), qPrintable(name + QLatin1Char(' ') + property));
        }
    }

    // Applied as BMainWindow::applyTheme() does it
    qApp->setStyleSheet(sheet);
    BStatusPalette::refresh();
    QVERIFY(BStatusPalette::instance());
    for (const Tone tone : kTones) {
        for (const Part part : kParts) {
            QCOMPARE(BStatus::color(tone, part), BStatus::defaultColor(tone, part, dark));
        }
    }
    // The palette widget reads them too
    QCOMPARE(BStatusPalette::instance()->successBackground(),
             BStatus::defaultColor(Tone::Success, Part::Background, dark));

    // The statistics bars use the accents
    QVERIFY(sheet.contains(QStringLiteral("QProgressBar[state=\"success\"]::chunk { background-color: %1; }")
                               .arg(BStatus::defaultColor(Tone::Success, Part::Accent, dark).name())));
}

void BStatusChipTest::tokensAreReadable()
{
    for (const bool dark : {true, false}) {
        QSet<QRgb> backgrounds;
        for (const Tone tone : kTones) {
            const QColor bg = BStatus::defaultColor(tone, Part::Background, dark);
            const QColor fg = BStatus::defaultColor(tone, Part::Foreground, dark);
            QVERIFY2(contrast(bg, fg) >= 4.5,
                     qPrintable(QStringLiteral("%1 %2: %3").arg(dark ? "dark" : "light",
                                                               BStatus::toneName(tone))
                                    .arg(contrast(bg, fg))));
            backgrounds.insert(bg.rgb());
        }
        // Every tone has a fill of its own
        QCOMPARE(backgrounds.size(), BStatus::ToneCount);
    }
}

// --- Delegate -----------------------------------------------------------------------------

namespace {

struct Cell {
    QStandardItemModel model{1, 1};
    BStatusDelegate delegate;
    QStyleOptionViewItem option;
    QImage image{300, 30, QImage::Format_ARGB32};

    explicit Cell(const QString &text)
    {
        model.setData(model.index(0, 0), text);
        option.rect = QRect(0, 0, 300, 30);
        option.font = QApplication::font();
        option.fontMetrics = QFontMetrics(option.font);
        option.palette = QApplication::palette();
        option.state = QStyle::State_Enabled;
        option.showDecorationSelected = true;
        option.decorationSize = QSize(16, 16);
    }

    QModelIndex index() const { return model.index(0, 0); }

    void paint()
    {
        image.fill(Qt::white);
        QPainter painter(&image);
        delegate.paint(&painter, option, index());
    }

    /// A pixel inside the chip, left of the code
    QColor chipPixel() const
    {
        const int x = BUiScale::px(4) + BUiScale::px(6) / 2;
        return image.pixelColor(x, image.height() / 2);
    }
};

} // namespace

void BStatusChipTest::delegatePaintsTheTone()
{
    Cell cell(QStringLiteral("Error"));
    cell.model.setData(cell.index(), int(Tone::Error), BStatus::ToneRole);
    cell.model.setData(cell.index(), QStringLiteral("E"), BStatus::CodeRole);
    cell.paint();
    QCOMPARE(cell.chipPixel(), BStatus::color(Tone::Error, Part::Background));

    // The cell right of the chip stays the cell's own background
    QCOMPARE(cell.image.pixelColor(295, 15), QColor(Qt::white));

    // Wide enough for code and word
    const QSize hint = cell.delegate.sizeHint(cell.option, cell.index());
    const QFontMetrics fm(cell.option.font);
    QVERIFY(hint.width() > fm.horizontalAdvance(QStringLiteral("E Error")));
    QVERIFY(hint.height() >= fm.height());

    // The success fill in the dark theme
    qApp->setStyleSheet(theme("dark"));
    BStatusPalette::refresh();
    cell.model.setData(cell.index(), int(Tone::Success), BStatus::ToneRole);
    cell.paint();
    QCOMPARE(cell.chipPixel(), BStatus::defaultColor(Tone::Success, Part::Background, true));
    const BStatusDelegate::Colors colors = BStatusDelegate::chipColors(cell.index());
    QCOMPARE(colors.foreground, BStatus::defaultColor(Tone::Success, Part::Foreground, true));
    QCOMPARE(colors.border, BStatus::defaultColor(Tone::Success, Part::Accent, true));
}

void BStatusChipTest::delegateKeepsTheSelection()
{
    Cell cell(QStringLiteral("Warnings"));
    cell.model.setData(cell.index(), int(Tone::Warning), BStatus::ToneRole);
    cell.model.setData(cell.index(), QStringLiteral("W"), BStatus::CodeRole);
    cell.option.state |= QStyle::State_Selected | QStyle::State_Active;
    cell.option.palette.setColor(QPalette::Highlight, QColor(9, 71, 113));
    cell.paint();

    // Selection around the chip, the chip's own fill inside
    QCOMPARE(cell.image.pixelColor(295, 15), QColor(9, 71, 113));
    QCOMPARE(cell.chipPixel(), BStatus::color(Tone::Warning, Part::Background));
}

void BStatusChipTest::delegateExplicitFill()
{
    // A backup level: the colour of the settings, black or white text
    Cell cell(QStringLiteral("Incr"));
    cell.model.setData(cell.index(), QColor(200, 255, 200), BStatus::ChipColorRole);
    cell.model.setData(cell.index(), QStringLiteral("I"), BStatus::CodeRole);
    const BStatusDelegate::Colors colors = BStatusDelegate::chipColors(cell.index());
    QCOMPARE(colors.background, QColor(200, 255, 200));
    QCOMPARE(colors.foreground, QColor(Qt::black));
    cell.paint();
    QCOMPARE(cell.chipPixel(), QColor(200, 255, 200));
}

void BStatusChipTest::delegateWithoutTone()
{
    Cell cell(QStringLiteral("plain"));
    QVERIFY(!BStatusDelegate::chipColors(cell.index()).background.isValid());
    cell.paint();
    // No chip: the default delegate paints text on the cell, no fill of a tone
    // (the neutral fill is a grey that anti-aliased text has as well)
    QSet<QRgb> fills;
    for (const Tone tone : kTones) {
        if (tone != Tone::Neutral) fills.insert(BStatus::color(tone, Part::Background).rgb());
    }
    for (int y = 0; y < cell.image.height(); ++y) {
        for (int x = 0; x < cell.image.width(); ++x) {
            QVERIFY(!fills.contains(cell.image.pixelColor(x, y).rgb()));
        }
    }
}

QTEST_MAIN(BStatusChipTest)
#include "bstatuschip_test.moc"
