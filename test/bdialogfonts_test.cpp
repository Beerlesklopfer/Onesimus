/**
 * @file bdialogfonts_test.cpp
 * @brief Dialogs take the interface font and zoom
 *
 * Windows of their own are not children of the main window: they take the
 * application font, and Qt keeps per-class fonts from the platform theme
 * besides it (QPushButton, QLabel, QMessageBox, item views, ...) that come
 * back with every application style sheet. Checked here, at 100 % and at
 * 200 % with a fixed base font:
 *
 * - every text widget of a plain dialog, a message box and the real job
 *   details, settings, connection and restore dialogs has the size of its
 *   role (the application font unless bound to another role);
 * - a zoom change while a dialog is open reaches it, and its size follows;
 * - a dialog made after the change is made at the new size;
 * - per-class fonts of the platform theme do not win;
 * - a theme style sheet with font-size and font-family of its own does not
 *   change the font.
 *
 * Offscreen, no Director, no network; the settings go to a temporary
 * directory. With ONESIMUS_DIALOG_SCREENSHOT_DIR set, pictures of the
 * dialogs are written there (dark and light, 100 % and 150 %).
 */

#include <QtTest>
#include <QAbstractButton>
#include <QAbstractItemView>
#include <QApplication>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QFile>
#include <QGroupBox>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPointer>
#include <QSettings>
#include <QTabBar>
#include <QTabWidget>
#include <QTemporaryDir>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWizard>

#include <functional>
#include <memory>

#include "bconnectionwizard.h"
#include "buiscale.h"
#include "config/bsettingsdialog.h"
#include "jobs/bjobdetailsdialog.h"
#include "jobs/bjoblogmodel.h"
#include "jobs/bjoblogview.h"
#include "jobs/brestorewizard.h"

namespace {

const qreal kBasePoints = 10.0;

QFont testFont()
{
    return QFont(QStringLiteral("DejaVu Sans"), int(kBasePoints));
}

QString readTheme(const QString &name)
{
    QFile file(QStringLiteral(ONESIMUS_THEME_DIR "/%1.qss").arg(name));
    if (!file.open(QIODevice::ReadOnly)) return QString();
    return QString::fromUtf8(file.readAll());
}

// What the platform theme of a desktop puts into Qt's per-class fonts:
// the desktop's size, not the zoomed one
void injectClassFonts()
{
    const QFont desktop(QStringLiteral("DejaVu Sans"), 7);
    for (const char *name : {"QPushButton", "QLabel", "QMessageBox", "QAbstractItemView",
                             "QHeaderView", "QCheckBox", "QRadioButton", "QToolButton"}) {
        QApplication::setFont(desktop, name);
    }
}

QJsonObject sampleJob()
{
    QJsonObject job;
    job[QStringLiteral("jobid")] = QStringLiteral("4711");
    job[QStringLiteral("name")] = QStringLiteral("BackupCatalog");
    job[QStringLiteral("client")] = QStringLiteral("pdc-fd");
    job[QStringLiteral("type")] = QStringLiteral("B");
    job[QStringLiteral("level")] = QStringLiteral("F");
    job[QStringLiteral("jobstatus")] = QStringLiteral("T");
    job[QStringLiteral("starttime")] = QStringLiteral("2026-11-18 23:10:00");
    job[QStringLiteral("duration")] = QStringLiteral("00:04:12");
    job[QStringLiteral("jobfiles")] = QStringLiteral("1284");
    job[QStringLiteral("jobbytes")] = QStringLiteral("734003200");
    job[QStringLiteral("fileset")] = QStringLiteral("Catalog");
    return job;
}

QWidget *makePlainDialog()
{
    auto *dialog = new QDialog;
    dialog->setWindowTitle(QStringLiteral("Plain dialog"));
    auto *layout = new QVBoxLayout(dialog);
    auto *tabs = new QTabWidget(dialog);
    auto *page = new QWidget;
    auto *pageLayout = new QVBoxLayout(page);
    pageLayout->addWidget(new QLabel(QStringLiteral("A label"), page));
    auto *group = new QGroupBox(QStringLiteral("A group"), page);
    auto *groupLayout = new QVBoxLayout(group);
    groupLayout->addWidget(new QLineEdit(QStringLiteral("Text"), group));
    pageLayout->addWidget(group);
    tabs->addTab(page, QStringLiteral("Files"));
    auto *list = new QListWidget;
    list->addItems({QStringLiteral("one"), QStringLiteral("two")});
    tabs->addTab(list, QStringLiteral("Status"));
    layout->addWidget(tabs);
    layout->addWidget(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog));
    dialog->resize(BUiScale::px(400), BUiScale::px(300));
    return dialog;
}

QWidget *makeMessageBox()
{
    auto *box = new QMessageBox(QMessageBox::Warning, QStringLiteral("Delete volume"),
                                QStringLiteral("Delete the volume Full-0042 from the catalog?"),
                                QMessageBox::Yes | QMessageBox::No);
    box->setInformativeText(QStringLiteral("The data on the volume stays where it is."));
    return box;
}

QWidget *makeJobDetails()
{
    auto *dialog = new BJobDetailsDialog(sampleJob(), nullptr, nullptr, nullptr);
    if (auto *view = dialog->findChild<BJobLogView *>()) {
        if (auto *model = qobject_cast<BJobLogModel *>(view->model())) {
            model->setLogLines({
                QStringLiteral("18-Nov 23:10 pdc-dir JobId 4711: Start Backup JobId 4711, Job=BackupCatalog.2026-11-18_23.10.00_03"),
                QStringLiteral("18-Nov 23:10 pdc-dir JobId 4711: Using Device \"FileStorage\" to write."),
                QStringLiteral("18-Nov 23:11 pdc-sd JobId 4711: Volume \"Full-0042\" previously written, moving to end of data."),
                QStringLiteral("18-Nov 23:14 pdc-sd JobId 4711: Warning: block.c:128 Volume nearly full"),
                QStringLiteral("18-Nov 23:14 pdc-dir JobId 4711: Bareos pdc-dir 24.0.1 (18Nov26):"),
                QStringLiteral("  Termination:            Backup OK"),
            });
        }
    }
    return dialog;
}

QWidget *makeSettings()
{
    return new BSettingsDialog(nullptr);
}

QWidget *makeConnectionWizard()
{
    return new BConnectionWizard(nullptr, nullptr, nullptr);
}

QWidget *makeRestoreWizard()
{
    return new BRestoreWizard(sampleJob(), nullptr, nullptr);
}

using Factory = std::function<QWidget *()>;

bool isTextWidget(const QWidget *widget)
{
    return qobject_cast<const QTabBar *>(widget) || qobject_cast<const QAbstractButton *>(widget)
        || qobject_cast<const QLabel *>(widget) || qobject_cast<const QAbstractItemView *>(widget)
        || qobject_cast<const QLineEdit *>(widget) || qobject_cast<const QTextEdit *>(widget)
        || qobject_cast<const QPlainTextEdit *>(widget) || qobject_cast<const QGroupBox *>(widget)
        || qobject_cast<const QComboBox *>(widget);
}

// The text widgets of a window, in the order of construction
QList<QWidget *> textWidgets(QWidget *window)
{
    QList<QWidget *> widgets{window};
    const QList<QWidget *> children = window->findChildren<QWidget *>();
    for (QWidget *child : children) {
        if (child->window() == window && isTextWidget(child)) {
            // A page not shown yet takes the fonts of its style sheet when
            // it is polished
            child->ensurePolished();
            widgets.append(child);
        }
    }
    return widgets;
}

// The factor of the role a widget has: its own binding or one it inherits
// from a bound ancestor (the title of a wizard page, a label Qt makes, is
// bound to Title by BUiScale)
qreal expectedFactor(const QWidget *widget)
{
    for (const QWidget *w = widget; w; w = w->isWindow() ? nullptr : w->parentWidget()) {
        const QVariant role = w->property("bUiFontRole");
        if (role.isValid()) return BUiScale::roleFactor(static_cast<BUiScale::Role>(role.toInt()));
    }
    return 1.0;
}

QString describe(const QWidget *widget)
{
    QString text;
    if (const auto *label = qobject_cast<const QLabel *>(widget)) text = label->text();
    else if (const auto *button = qobject_cast<const QAbstractButton *>(widget)) text = button->text();
    else if (const auto *group = qobject_cast<const QGroupBox *>(widget)) text = group->title();
    return QStringLiteral("%1 \"%2\" (%3) in %4")
        .arg(QLatin1String(widget->metaObject()->className()), text.left(40), widget->objectName(),
             widget->parentWidget() ? QLatin1String(widget->parentWidget()->metaObject()->className())
                                    : QLatin1String("-"));
}

// Every text widget has the size of its role at the current zoom
QString wrongSizes(QWidget *window)
{
    const qreal app = BUiScale::instance().applicationFont().pointSizeF();
    QStringList wrong;
    for (QWidget *widget : textWidgets(window)) {
        const qreal expected = app * expectedFactor(widget);
        const qreal actual = widget->font().pointSizeF();
        if (qAbs(actual - expected) > 0.05)
            wrong << QStringLiteral("%1: %2 pt instead of %3 pt").arg(describe(widget)).arg(actual).arg(expected);
    }
    return wrong.join(QLatin1Char('\n'));
}

void showAndSettle(QWidget *window)
{
    window->show();
    QCoreApplication::processEvents();
    QCoreApplication::processEvents();
}

} // namespace

Q_DECLARE_METATYPE(Factory)

class BDialogFontsTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void init();
    void cleanup();

    void themeCannotSetTheFont();
    void messageBoxShowsItsWholeText();
    void styleSheetDropsClassFonts();
    void classFontsDoNotReachANewDialog();

    void dialogsFollowTheZoom_data();
    void dialogsFollowTheZoom();

    void screenshots();

private:
    void addFactories();
    QTemporaryDir m_config;
};

void BDialogFontsTest::initTestCase()
{
    QVERIFY(m_config.isValid());
    qputenv("XDG_CONFIG_HOME", m_config.path().toUtf8());
    qputenv("XDG_DATA_HOME", m_config.path().toUtf8());
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, m_config.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_config.path());
    // Windows keeps NativeFormat in the registry, where setPath does not reach
    QSettings::setDefaultFormat(QSettings::IniFormat);
    const QSettings probe(QSettings::defaultFormat(), QSettings::UserScope,
                         QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
    QVERIFY2(probe.fileName().startsWith(m_config.path()), qPrintable(probe.fileName()));

    BUiScale::instance().setBaseFont(testFont());
    const QString dark = readTheme(QStringLiteral("dark"));
    QVERIFY(!dark.isEmpty());
    BUiScale::instance().setApplicationStyleSheet(dark + BUiScale::themeRules());
}

void BDialogFontsTest::init()
{
    BUiScale::instance().resetZoom();
}

void BDialogFontsTest::cleanup()
{
    QList<QPointer<QWidget>> windows;
    for (QWidget *window : QApplication::topLevelWidgets()) windows.append(window);
    for (const QPointer<QWidget> &window : std::as_const(windows)) delete window.data();
}

void BDialogFontsTest::messageBoxShowsItsWholeText()
{
    // The connection error came cut off ("gRPC login failed:" and no reason,
    // the title "Connection Erro"): the box had measured itself before the
    // interface font reached it
    BUiScale::instance().setZoomPercent(150);
    auto *box = new QMessageBox(QMessageBox::Critical, QStringLiteral("Connection Error"),
                                QStringLiteral("gRPC login failed: UNAVAILABLE: the gRPC service of the "
                                               "Director cannot be reached (connection refused)"),
                                QMessageBox::Ok);
    box->setAttribute(Qt::WA_DeleteOnClose);
    showAndSettle(box);
    QCoreApplication::processEvents();

    const QLabel *label = box->findChild<QLabel *>(QStringLiteral("qt_msgbox_label"));
    QVERIFY(label);
    QCOMPARE(label->font().pointSizeF(), 1.5 * kBasePoints);
    const int needed = label->wordWrap() ? label->heightForWidth(label->width())
                                         : label->sizeHint().height();
    QVERIFY2(label->height() >= needed,
             qPrintable(QStringLiteral("label %1 px high, text needs %2").arg(label->height()).arg(needed)));
    if (!label->wordWrap()) {
        QVERIFY2(label->width() >= label->sizeHint().width(),
                 qPrintable(QStringLiteral("label %1 px wide, text needs %2")
                                .arg(label->width()).arg(label->sizeHint().width())));
    }
    QVERIFY2(box->width() >= box->sizeHint().width(),
             qPrintable(QStringLiteral("box %1 px wide, needs %2").arg(box->width()).arg(box->sizeHint().width())));
    box->close();
}

void BDialogFontsTest::themeCannotSetTheFont()
{
    // An old copy of a theme, as a user's custom theme could hold it
    const QString custom = QStringLiteral(
        "QWidget { font-size: 10pt; font-family: \"Segoe UI\", sans-serif; color: #ddd; }\n"
        "QLabel#big { font: bold 14px Arial; }\n"
        "QPushButton { FONT-SIZE:12px; font-weight: bold; font-style: italic }\n"
        "QLabel#hint { font-size: @small; }\n");
    const QString filtered = BUiScale::themeCss(custom);
    QVERIFY2(!filtered.contains(QLatin1String("10pt")), qPrintable(filtered));
    QVERIFY2(!filtered.contains(QLatin1String("Segoe")), qPrintable(filtered));
    QVERIFY2(!filtered.contains(QLatin1String("Arial")), qPrintable(filtered));
    QVERIFY2(!filtered.contains(QLatin1String("12px")), qPrintable(filtered));
    QVERIFY2(filtered.contains(QLatin1String("font-weight: bold")), qPrintable(filtered));
    QVERIFY2(filtered.contains(QLatin1String("font-style: italic")), qPrintable(filtered));
    QVERIFY2(filtered.contains(QLatin1String("color: #ddd")), qPrintable(filtered));
    QVERIFY2(filtered.contains(QLatin1String("font-size: 9.0pt")), qPrintable(filtered));

    // Applied: the interface font and zoom stay in charge
    BUiScale::instance().setZoomPercent(200);
    BUiScale::instance().setApplicationStyleSheet(readTheme(QStringLiteral("dark")) + custom);
    std::unique_ptr<QWidget> dialog(makePlainDialog());
    showAndSettle(dialog.get());
    const QString wrong = wrongSizes(dialog.get());
    QVERIFY2(wrong.isEmpty(), qPrintable(wrong));
    const QLabel *label = dialog->findChild<QLabel *>();
    QVERIFY(label);
    QCOMPARE(label->font().pointSizeF(), 2 * kBasePoints);
    QCOMPARE(label->font().family(), testFont().family());
    const QPushButton *button = dialog->findChild<QPushButton *>();
    QVERIFY(button);
    QCOMPARE(button->font().pointSizeF(), 2 * kBasePoints);

    BUiScale::instance().setApplicationStyleSheet(readTheme(QStringLiteral("dark")) + BUiScale::themeRules());
}

void BDialogFontsTest::styleSheetDropsClassFonts()
{
    BUiScale::instance().setZoomPercent(150);
    injectClassFonts();
    QVERIFY(QApplication::font("QPushButton") != QApplication::font());
    BUiScale::instance().setApplicationStyleSheet(readTheme(QStringLiteral("dark")) + BUiScale::themeRules());
    for (const char *name : {"QPushButton", "QLabel", "QMessageBox", "QAbstractItemView", "QHeaderView"})
        QCOMPARE(QApplication::font(name), BUiScale::instance().applicationFont());
}

void BDialogFontsTest::classFontsDoNotReachANewDialog()
{
    // As after a QApplication::setStyleSheet() that did not go through
    // BUiScale: a window that is shown drops them
    BUiScale::instance().setZoomPercent(150);
    injectClassFonts();
    std::unique_ptr<QWidget> box(makeMessageBox());
    showAndSettle(box.get());
    QString wrong = wrongSizes(box.get());
    QVERIFY2(wrong.isEmpty(), qPrintable(wrong));
    QCOMPARE(QApplication::font("QPushButton"), BUiScale::instance().applicationFont());

    injectClassFonts();
    std::unique_ptr<QWidget> dialog(makePlainDialog());
    showAndSettle(dialog.get());
    wrong = wrongSizes(dialog.get());
    QVERIFY2(wrong.isEmpty(), qPrintable(wrong));
}

void BDialogFontsTest::addFactories()
{
    QTest::addColumn<Factory>("factory");
    QTest::addColumn<bool>("scalesItsSize");
    QTest::newRow("plain dialog") << Factory(makePlainDialog) << true;
    QTest::newRow("message box") << Factory(makeMessageBox) << false;
    QTest::newRow("job details") << Factory(makeJobDetails) << true;
    QTest::newRow("settings") << Factory(makeSettings) << true;
    QTest::newRow("connection wizard") << Factory(makeConnectionWizard) << true;
    QTest::newRow("restore wizard") << Factory(makeRestoreWizard) << true;
}

void BDialogFontsTest::dialogsFollowTheZoom_data()
{
    addFactories();
}

void BDialogFontsTest::dialogsFollowTheZoom()
{
    QFETCH(Factory, factory);
    QFETCH(bool, scalesItsSize);
    BUiScale &scale = BUiScale::instance();

    // At 100 %, with the per-class fonts a desktop gives
    injectClassFonts();
    std::unique_ptr<QWidget> before(factory());
    showAndSettle(before.get());
    QString wrong = wrongSizes(before.get());
    QVERIFY2(wrong.isEmpty(), qPrintable(QStringLiteral("at 100 %:\n") + wrong));
    const QSize sizeAt100 = before->size();
    QList<qreal> pointsAt100;
    for (QWidget *widget : textWidgets(before.get())) pointsAt100 << widget->font().pointSizeF();

    // The zoom changes while it is open; the theme is set again as the main
    // window does
    scale.setZoomPercent(200);
    scale.setApplicationStyleSheet(readTheme(QStringLiteral("dark")) + BUiScale::themeRules());
    QCoreApplication::processEvents();
    wrong = wrongSizes(before.get());
    QVERIFY2(wrong.isEmpty(), qPrintable(QStringLiteral("open during the change to 200 %:\n") + wrong));
    const QList<QWidget *> live = textWidgets(before.get());
    QCOMPARE(live.size(), pointsAt100.size());
    for (int i = 0; i < live.size(); ++i)
        QCOMPARE(live.at(i)->font().pointSizeF(), 2 * pointsAt100.at(i));
    if (scalesItsSize) {
        QVERIFY2(qAbs(before->width() - 2 * sizeAt100.width()) <= 2,
                 qPrintable(QStringLiteral("%1 -> %2").arg(sizeAt100.width()).arg(before->width())));
        QVERIFY2(qAbs(before->height() - 2 * sizeAt100.height()) <= 2,
                 qPrintable(QStringLiteral("%1 -> %2").arg(sizeAt100.height()).arg(before->height())));
    } else {
        QVERIFY(before->width() >= sizeAt100.width());
    }

    // Made after the change
    injectClassFonts();
    std::unique_ptr<QWidget> after(factory());
    showAndSettle(after.get());
    wrong = wrongSizes(after.get());
    QVERIFY2(wrong.isEmpty(), qPrintable(QStringLiteral("made at 200 %:\n") + wrong));
    const QList<QWidget *> made = textWidgets(after.get());
    QCOMPARE(made.size(), pointsAt100.size());
    for (int i = 0; i < made.size(); ++i)
        QCOMPARE(made.at(i)->font().pointSizeF(), 2 * pointsAt100.at(i));
    if (scalesItsSize) {
        QVERIFY2(qAbs(after->width() - 2 * sizeAt100.width()) <= 2,
                 qPrintable(QStringLiteral("%1 -> %2").arg(sizeAt100.width()).arg(after->width())));
    }
}

void BDialogFontsTest::screenshots()
{
    const QString dir = qEnvironmentVariable("ONESIMUS_DIALOG_SCREENSHOT_DIR");
    if (dir.isEmpty()) QSKIP("ONESIMUS_DIALOG_SCREENSHOT_DIR is not set");
    QVERIFY(QDir().mkpath(dir));

    const struct { const char *name; Factory factory; int tabs; } dialogs[] = {
        {"job-details", makeJobDetails, 3},
        {"settings", makeSettings, 0},
        {"message-box", makeMessageBox, 0},
        {"connection-wizard", makeConnectionWizard, 0},
        {"restore-wizard", makeRestoreWizard, 0},
    };
    for (const char *theme : {"dark", "light"}) {
        for (int zoom : {100, 150}) {
            BUiScale::instance().setZoomPercent(zoom);
            BUiScale::instance().setApplicationStyleSheet(readTheme(QLatin1String(theme))
                                                          + BUiScale::themeRules());
            for (const auto &entry : dialogs) {
                std::unique_ptr<QWidget> window(entry.factory());
                showAndSettle(window.get());
                auto save = [&](const QString &suffix) {
                    QCoreApplication::processEvents();
                    const QString file = QStringLiteral("%1/dlg-%2%3-%4-%5.png")
                        .arg(dir, QLatin1String(entry.name), suffix).arg(zoom).arg(QLatin1String(theme));
                    QVERIFY2(window->grab().save(file), qPrintable(file));
                };
                if (entry.tabs > 0) {
                    auto *tabs = window->findChild<QTabWidget *>();
                    QVERIFY(tabs);
                    for (int i = 0; i < entry.tabs; ++i) {
                        tabs->setCurrentIndex(i);
                        save(QLatin1Char('-') + tabs->tabText(i).toLower());
                    }
                } else {
                    save(QString());
                }
            }
        }
    }
}

// The offscreen screen is 800 x 800 by default: too small for a dialog at
// 200 %, whose size would be limited to it. A larger one is configured
// before the application starts.
int main(int argc, char *argv[])
{
    QTemporaryDir platformDir;
    QString previousDir;
    if (platformDir.isValid() && qEnvironmentVariable("QT_QPA_PLATFORM") == QLatin1String("offscreen")) {
        const QString config = platformDir.filePath(QStringLiteral("screens.json"));
        QFile file(config);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(R"({"screens":[{"name":"test","x":0,"y":0,"width":3200,"height":2400,)"
                       R"("logicalDpi":96,"logicalBaseDpi":96,"dpr":1}]})");
            file.close();
            // The platform arguments are split at ':', so "C:/..." would break
            // apart on Windows: a name relative to the current directory instead
            previousDir = QDir::currentPath();
            QDir::setCurrent(platformDir.path());
            qputenv("QT_QPA_PLATFORM", QByteArrayLiteral("offscreen:configfile=screens.json"));
        }
    }
    QApplication app(argc, argv);
    if (!previousDir.isEmpty()) {
        QDir::setCurrent(previousDir);
    }
    BDialogFontsTest test;
    QTEST_SET_MAIN_SOURCE_PATH
    return QTest::qExec(&test, argc, argv);
}

#include "bdialogfonts_test.moc"
