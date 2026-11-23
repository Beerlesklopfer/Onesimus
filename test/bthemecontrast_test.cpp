// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file bthemecontrast_test.cpp
 * @brief Text is readable in every theme, on every desktop (issue #6)
 *
 * For each theme the application offers (light, dark, and "system" with the
 * desktop forced to light and to dark) the real main window is built and
 * applies its theme (BMainWindow::applyTheme()) on a hostile desktop: a
 * non-Fusion style and a palette of the opposite brightness, as a desktop
 * whose colour scheme does not match the theme would give it. Then the
 * widget types the application uses are built and the contrast between
 * text and background is measured twice:
 *
 * - from the resolved palette of the polished widget (the style sheet sets
 *   it), and
 * - from a rendering: the widget is grabbed and the most frequent colour of
 *   the text's rectangle is taken as background, the colour that differs
 *   most from it (reached by a few pixels at least) as text. That catches a
 *   style sheet rule that paints over the palette.
 *
 * Required is the WCAG 2.x ratio for normal text, 4.5:1. Deliberate
 * exceptions (WCAG 1.4.3 exempts "inactive user interface components"):
 * disabled controls only have to stay visible (2:1).
 *
 * Offscreen, no Director, no network; settings and data go to a temporary
 * directory. With ONESIMUS_THEME_SCREENSHOT_DIR set, pictures of the main
 * window's tabs, the widget gallery and a few dialogs are written there for
 * each theme, together with the measured table (contrast-<theme>.txt).
 */

#include <QtTest>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDir>
#include <QDirIterator>
#include <QGroupBox>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRegularExpression>
#include <QSettings>
#include <QSpinBox>
#include <QStandardPaths>
#include <QStatusBar>
#include <QStyleFactory>
#include <QStyleHints>
#include <QStyleOptionComboBox>
#include <QStyleOptionGroupBox>
#include <QTabBar>
#include <QTabWidget>
#include <QTableView>
#include <QTableWidget>
#include <QTemporaryDir>
#include <QToolTip>
#include <QTreeWidget>
#include <QVBoxLayout>

#include <algorithm>
#include <memory>

#include "bcertificategenerator.h"
#include "bconnectionwizard.h"
#include "blabelstate.h"
#include "bstatuschip.h"
#include "bmainwindow.h"
#include "bpfxconverter.h"
#include "config/bsettings.h"
#include "config/bsettingsdialog.h"
#include "jobs/bjobdetailsdialog.h"
#include "jobs/bjoblogmodel.h"
#include "jobs/bjoblogview.h"
#include "jobs/bjobwidget.h"

namespace {

constexpr double NormalText = 4.5;   // WCAG 2.x, 1.4.3, normal text
constexpr double Disabled = 2.0;     // exempt by WCAG; only has to stay visible

double luminance(const QColor &c)
{
    auto ch = [](double v) {
        v /= 255.0;
        return v <= 0.03928 ? v / 12.92 : std::pow((v + 0.055) / 1.055, 2.4);
    };
    return 0.2126 * ch(c.red()) + 0.7152 * ch(c.green()) + 0.0722 * ch(c.blue());
}

/// @p fg painted over @p bg (the style sheet gives placeholders half alpha)
QColor over(const QColor &fg, const QColor &bg)
{
    const double a = fg.alphaF();
    return QColor::fromRgbF(float(fg.redF() * a + bg.redF() * (1 - a)),
                            float(fg.greenF() * a + bg.greenF() * (1 - a)),
                            float(fg.blueF() * a + bg.blueF() * (1 - a)));
}

double contrast(const QColor &a, const QColor &b)
{
    const double la = luminance(a) + 0.05;
    const double lb = luminance(b) + 0.05;
    return la > lb ? la / lb : lb / la;
}

/// Text and background colour read from a rendering
struct Rendered
{
    QColor background;
    QColor text;
    double ratio = 1.0;
};

Rendered sampleRendering(const QImage &image, QRect rect)
{
    rect = rect.intersected(image.rect());
    Rendered r;
    if (rect.isEmpty())
        return r;

    QHash<QRgb, int> histogram;
    for (int y = rect.top(); y <= rect.bottom(); ++y)
        for (int x = rect.left(); x <= rect.right(); ++x)
            ++histogram[image.pixel(x, y) | 0xff000000u];
    QRgb background = 0;
    int most = -1;
    for (auto it = histogram.cbegin(); it != histogram.cend(); ++it) {
        if (it.value() > most) {
            most = it.value();
            background = it.key();
        }
    }
    r.background = QColor::fromRgb(background);

    // The text colour: the one that differs most from the background and is
    // reached by at least a few pixels (glyph stems, not a stray border pixel)
    struct Px { double ratio; QRgb rgb; };
    QList<Px> pixels;
    for (auto it = histogram.cbegin(); it != histogram.cend(); ++it)
        pixels.append({contrast(QColor::fromRgb(it.key()), r.background), it.key()});
    std::sort(pixels.begin(), pixels.end(), [](const Px &a, const Px &b) { return a.ratio > b.ratio; });
    int count = 0;
    for (const Px &px : std::as_const(pixels)) {
        count += histogram.value(px.rgb);
        if (count >= 4) {
            r.text = QColor::fromRgb(px.rgb);
            r.ratio = px.ratio;
            break;
        }
    }
    return r;
}

QJsonDocument jobPage()
{
    // One job of each colour the job table knows
    const struct { const char *status; const char *level; const char *type; } jobs[] = {
        {"T", "F", "B"}, {"T", "I", "B"}, {"T", "D", "B"}, {"W", "I", "B"},
        {"E", "F", "B"}, {"f", "I", "B"}, {"A", "F", "B"}, {"R", "I", "B"},
        {"C", "F", "B"}, {"T", "F", "R"}, {"T", "F", "C"},
    };
    QJsonArray array;
    int id = 100;
    for (const auto &job : jobs) {
        array.append(QJsonObject{{"jobid", QString::number(id)},
                                 {"name", QStringLiteral("backup-%1").arg(id)},
                                 {"client", "pdc-fd"}, {"level", job.level}, {"type", job.type},
                                 {"jobstatus", job.status}, {"starttime", "2026-11-20 01:00:00"},
                                 {"endtime", "2026-11-20 01:10:00"},
                                 {"jobbytes", "734003200"}, {"jobfiles", "1284"}});
        ++id;
    }
    return QJsonDocument(QJsonObject{{"jsonrpc", "2.0"},
                                     {"result", QJsonObject{{"jobs", array}}}});
}

QStringList logLines()
{
    return {
        QStringLiteral("18-Nov 23:10 pdc-dir JobId 4711: Start Backup JobId 4711"),
        QStringLiteral("18-Nov 23:11 pdc-sd JobId 4711: Warning: Volume nearly full"),
        QStringLiteral("18-Nov 23:12 pdc-fd JobId 4711: Error: Could not stat \"/srv/x\""),
        QStringLiteral("18-Nov 23:13 pdc-dir JobId 4711: Fatal error: Network error"),
        QStringLiteral("18-Nov 23:14 pdc-dir JobId 4711: Bareos pdc-dir 24.0.1:"),
        QStringLiteral("  Termination:            Backup OK"),
        QStringLiteral("18-Nov 23:15 pdc-dir JobId 4711: End of job"),
    };
}

void settle(QWidget *w)
{
    w->show();
    w->activateWindow();
    QVERIFY(QTest::qWaitForWindowExposed(w));
    for (int i = 0; i < 5; ++i) {
        QCoreApplication::processEvents();
        QTest::qWait(5);
    }
}

void hover(QAbstractItemView *view, const QPoint &pos)
{
    QHoverEvent enter(QEvent::HoverEnter, pos, view->viewport()->mapToGlobal(pos), QPointF(-1, -1));
    QCoreApplication::sendEvent(view->viewport(), &enter);
    QHoverEvent move(QEvent::HoverMove, pos, view->viewport()->mapToGlobal(pos), pos);
    QCoreApplication::sendEvent(view->viewport(), &move);
    view->viewport()->update();
    QCoreApplication::processEvents();
}

} // namespace

class BThemeContrastTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void contrast_data();
    void contrast();
    void noFixedTextColours();

private:
    struct Row
    {
        QString widget;
        double palette = 0;    ///< 0: not measured
        double rendered = 0;   ///< 0: not measured
        double required = NormalText;
        QString note;
    };

    void check(const QString &widget, double required, const QColor &fg, const QColor &bg,
               const QImage &image, const QRect &rect, const QString &note = QString());
    void checkPalette(const QString &widget, double required, const QColor &fg, const QColor &bg,
                      const QString &note = QString());
    void checkGallery(QWidget *window);
    void checkItemViews(QWidget *window);
    void checkPopups(QWidget *window);
    void checkAppWidgets(BMainWindow *mainWindow);
    void screenshots(BMainWindow *mainWindow, QWidget *gallery, const QString &theme);

    QTemporaryDir m_config;
    QList<Row> m_rows;
};

void BThemeContrastTest::initTestCase()
{
    QVERIFY(m_config.isValid());
    QStandardPaths::setTestModeEnabled(true);
    qputenv("XDG_CONFIG_HOME", m_config.path().toUtf8());
    qputenv("XDG_DATA_HOME", m_config.filePath(QStringLiteral("data")).toUtf8());
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, m_config.path());
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, m_config.path());
    QSettings::setDefaultFormat(QSettings::IniFormat);
    const QSettings probe(QSettings::defaultFormat(), QSettings::UserScope,
                          QStringLiteral("Bareos"), QStringLiteral("Onesimus"));
    QVERIFY2(probe.fileName().startsWith(m_config.path()), qPrintable(probe.fileName()));
    BSettings::instance().setConnectionAutoConnect(false);
}

void BThemeContrastTest::contrast_data()
{
    QTest::addColumn<QString>("setting");
    QTest::addColumn<int>("desktop");      // Qt::ColorScheme forced for the desktop
    QTest::addColumn<QString>("expected"); // theme on display

    QTest::newRow("light") << QStringLiteral("light") << int(Qt::ColorScheme::Dark) << QStringLiteral("light");
    QTest::newRow("dark") << QStringLiteral("dark") << int(Qt::ColorScheme::Light) << QStringLiteral("dark");
    QTest::newRow("system-light") << QStringLiteral("system") << int(Qt::ColorScheme::Light) << QStringLiteral("light");
    QTest::newRow("system-dark") << QStringLiteral("system") << int(Qt::ColorScheme::Dark) << QStringLiteral("dark");
    // The offscreen platform cannot force a scheme: it reports "unknown",
    // which "system" shows as light (BTheme::systemIsDark())
    QTest::newRow("system-unknown") << QStringLiteral("system") << int(Qt::ColorScheme::Unknown) << QStringLiteral("light");
}

void BThemeContrastTest::checkPalette(const QString &widget, double required, const QColor &fg,
                                      const QColor &bg, const QString &note)
{
    Row row;
    row.widget = widget;
    row.palette = ::contrast(over(fg, bg), bg);
    row.required = required;
    row.note = note.isEmpty() ? QStringLiteral("%1 on %2").arg(over(fg, bg).name(), bg.name()) : note;
    m_rows.append(row);
}

void BThemeContrastTest::check(const QString &widget, double required, const QColor &fg,
                               const QColor &bg, const QImage &image, const QRect &rect,
                               const QString &note)
{
    Row row;
    row.widget = widget;
    row.required = required;
    if (fg.isValid() && bg.isValid())
        row.palette = ::contrast(over(fg, bg), bg);
    const Rendered r = sampleRendering(image, rect);
    row.rendered = r.ratio;
    row.note = note;
    if (row.note.isEmpty()) {
        row.note = QStringLiteral("rendered %1 on %2").arg(r.text.name(), r.background.name());
        if (fg.isValid() && bg.isValid())
            row.note += QStringLiteral("; palette %1 on %2").arg(over(fg, bg).name(), bg.name());
    }
    m_rows.append(row);
}

// Standard widgets of the kinds the application uses, in a dialog of their own
void BThemeContrastTest::checkGallery(QWidget *window)
{
    const QImage image = window->grab().toImage();
    auto at = [window](QWidget *w, QRect r = QRect()) {
        if (r.isNull())
            r = w->rect();
        return QRect(w->mapTo(window, r.topLeft()), r.size());
    };
    auto inset = [](QRect r, int dx, int dy) { return r.adjusted(dx, dy, -dx, -dy); };
    const QColor windowBg = window->palette().color(QPalette::Window);

    auto *label = window->findChild<QLabel *>(QStringLiteral("plainLabel"));
    check(QStringLiteral("QLabel"), NormalText, label->palette().color(QPalette::WindowText),
          windowBg, image, at(label));

    for (const char *state : {"success", "info", "warning", "error", "muted", "active"}) {
        auto *l = window->findChild<QLabel *>(QStringLiteral("state-%1").arg(QLatin1String(state)));
        check(QStringLiteral("QLabel[state=%1]").arg(QLatin1String(state)), NormalText,
              l->palette().color(QPalette::WindowText), windowBg, image, at(l));
    }

    auto *edit = window->findChild<QLineEdit *>(QStringLiteral("edit"));
    check(QStringLiteral("QLineEdit"), NormalText, edit->palette().color(QPalette::Text),
          edit->palette().color(QPalette::Base), image, at(edit, inset(edit->rect(), 4, 4)));
    auto *placeholder = window->findChild<QLineEdit *>(QStringLiteral("placeholder"));
    check(QStringLiteral("QLineEdit placeholder"), NormalText,
          placeholder->palette().color(QPalette::PlaceholderText),
          placeholder->palette().color(QPalette::Base), image, at(placeholder, inset(placeholder->rect(), 4, 4)));
    auto *disabledEdit = window->findChild<QLineEdit *>(QStringLiteral("disabledEdit"));
    check(QStringLiteral("QLineEdit disabled (exception)"), Disabled,
          disabledEdit->palette().color(QPalette::Disabled, QPalette::Text),
          disabledEdit->palette().color(QPalette::Disabled, QPalette::Base), image,
          at(disabledEdit, inset(disabledEdit->rect(), 4, 4)));

    auto *spin = window->findChild<QSpinBox *>(QStringLiteral("spin"));
    QLineEdit *spinEdit = spin->findChild<QLineEdit *>();
    check(QStringLiteral("QSpinBox"), NormalText, spinEdit->palette().color(QPalette::Text),
          spinEdit->palette().color(QPalette::Base), image, at(spinEdit));

    auto *combo = window->findChild<QComboBox *>(QStringLiteral("combo"));
    QStyleOptionComboBox comboOpt;
    comboOpt.initFrom(combo);
    const QRect field = combo->style()->subControlRect(QStyle::CC_ComboBox, &comboOpt,
                                                       QStyle::SC_ComboBoxEditField, combo);
    check(QStringLiteral("QComboBox"), NormalText, combo->palette().color(QPalette::ButtonText),
          combo->palette().color(QPalette::Button), image, at(combo, field));

    auto *tabs = window->findChild<QTabWidget *>(QStringLiteral("tabs"));
    QTabBar *bar = tabs->tabBar();
    check(QStringLiteral("QTabBar selected tab"), NormalText, QColor(), QColor(), image,
          at(bar, inset(bar->tabRect(bar->currentIndex()), 2, 2)));
    const int other = bar->currentIndex() == 0 ? 1 : 0;
    check(QStringLiteral("QTabBar other tab"), NormalText, QColor(), QColor(), image,
          at(bar, inset(bar->tabRect(other), 2, 2)));

    auto *group = window->findChild<QGroupBox *>(QStringLiteral("group"));
    QStyleOptionGroupBox groupOpt;
    groupOpt.initFrom(group);
    groupOpt.text = group->title();
    groupOpt.subControls = QStyle::SC_GroupBoxLabel;
    groupOpt.lineWidth = 1;
    const QRect title = group->style()->subControlRect(QStyle::CC_GroupBox, &groupOpt,
                                                       QStyle::SC_GroupBoxLabel, group);
    check(QStringLiteral("QGroupBox title"), NormalText, group->palette().color(QPalette::WindowText),
          windowBg, image, at(group, title));

    auto *check1 = window->findChild<QCheckBox *>(QStringLiteral("check"));
    check(QStringLiteral("QCheckBox"), NormalText, check1->palette().color(QPalette::WindowText),
          windowBg, image, at(check1, QRect(check1->width() / 3, 0, check1->width() * 2 / 3, check1->height())));

    for (const char *name : {"button", "defaultButton"}) {
        auto *b = window->findChild<QPushButton *>(QLatin1String(name));
        check(QStringLiteral("QPushButton %1").arg(QLatin1String(name)), NormalText,
              b->palette().color(QPalette::ButtonText), b->palette().color(QPalette::Button),
              image, at(b, inset(b->rect(), 3, 3)));
    }
    auto *disabledButton = window->findChild<QPushButton *>(QStringLiteral("disabledButton"));
    check(QStringLiteral("QPushButton disabled (exception)"), Disabled,
          disabledButton->palette().color(QPalette::Disabled, QPalette::ButtonText),
          disabledButton->palette().color(QPalette::Disabled, QPalette::Button), image,
          at(disabledButton, inset(disabledButton->rect(), 3, 3)));

    auto *box = window->findChild<QDialogButtonBox *>();
    for (QAbstractButton *b : box->buttons()) {
        check(QStringLiteral("QDialogButtonBox %1").arg(b->text().remove(QLatin1Char('&'))), NormalText,
              b->palette().color(QPalette::ButtonText), b->palette().color(QPalette::Button),
              image, at(b, inset(b->rect(), 3, 3)));
    }

    auto *text = window->findChild<QPlainTextEdit *>(QStringLiteral("plainText"));
    check(QStringLiteral("QPlainTextEdit"), NormalText, text->palette().color(QPalette::Text),
          text->palette().color(QPalette::Base), image, at(text->viewport()));
}

void BThemeContrastTest::checkItemViews(QWidget *window)
{
    auto *table = window->findChild<QTableWidget *>(QStringLiteral("table"));
    auto *tree = window->findChild<QTreeWidget *>(QStringLiteral("tree"));
    const QPalette tp = table->palette();
    table->selectRow(2);
    hover(table, table->visualRect(table->model()->index(3, 0)).center());
    const QImage image = table->viewport()->grab().toImage();
    auto cell = [&](int r) { return table->visualRect(table->model()->index(r, 0)).adjusted(2, 2, -2, -2); };
    check(QStringLiteral("QTableView row"), NormalText, tp.color(QPalette::Text), tp.color(QPalette::Base), image, cell(0));
    check(QStringLiteral("QTableView alternate row"), NormalText, tp.color(QPalette::Text),
          tp.color(QPalette::AlternateBase), image, cell(1));
    check(QStringLiteral("QTableView selected row"), NormalText, tp.color(QPalette::HighlightedText),
          tp.color(QPalette::Highlight), image, cell(2));
    check(QStringLiteral("QTableView hovered row"), NormalText, QColor(), QColor(), image, cell(3));

    QHeaderView *header = table->horizontalHeader();
    const QImage headerImage = header->grab().toImage();
    const int x = header->sectionViewportPosition(0);
    check(QStringLiteral("QHeaderView section"), NormalText, QColor(), QColor(), headerImage,
          QRect(x + 2, 2, header->sectionSize(0) - 4, header->height() - 4));

    tree->expandAll();
    QTreeWidgetItem *child = tree->topLevelItem(0)->child(0);
    tree->setCurrentItem(child);
    hover(tree, tree->visualItemRect(tree->topLevelItem(1)).center());
    const QImage treeImage = tree->viewport()->grab().toImage();
    const QPalette rp = tree->palette();
    check(QStringLiteral("QTreeView item"), NormalText, rp.color(QPalette::Text), rp.color(QPalette::Base),
          treeImage, tree->visualItemRect(tree->topLevelItem(0)).adjusted(2, 2, -2, -2));
    check(QStringLiteral("QTreeView selected item"), NormalText, rp.color(QPalette::HighlightedText),
          rp.color(QPalette::Highlight), treeImage, tree->visualItemRect(child).adjusted(2, 2, -2, -2));
    check(QStringLiteral("QTreeView hovered item"), NormalText, QColor(), QColor(), treeImage,
          tree->visualItemRect(tree->topLevelItem(1)).adjusted(2, 2, -2, -2));
}

void BThemeContrastTest::checkPopups(QWidget *window)
{
    // Combo box popup
    auto *combo = window->findChild<QComboBox *>(QStringLiteral("combo"));
    combo->showPopup();
    QAbstractItemView *view = combo->view();
    QVERIFY(QTest::qWaitForWindowExposed(view->window()));
    view->setCurrentIndex(view->model()->index(2, 0));
    hover(view, view->visualRect(view->model()->index(1, 0)).center());
    QCoreApplication::processEvents();
    const QImage popup = view->viewport()->grab().toImage();
    const QPalette vp = view->palette();
    check(QStringLiteral("QComboBox popup item"), NormalText, vp.color(QPalette::Text), vp.color(QPalette::Base),
          popup, view->visualRect(view->model()->index(0, 0)).adjusted(2, 1, -2, -1));
    check(QStringLiteral("QComboBox popup hovered item"), NormalText, QColor(), QColor(),
          popup, view->visualRect(view->model()->index(1, 0)).adjusted(2, 1, -2, -1));
    check(QStringLiteral("QComboBox popup current item"), NormalText, vp.color(QPalette::HighlightedText),
          vp.color(QPalette::Highlight), popup, view->visualRect(view->model()->index(2, 0)).adjusted(2, 1, -2, -1));
    combo->hidePopup();

    // Menu
    QMenu menu(window);
    menu.addAction(QStringLiteral("Refresh jobs"));
    QAction *active = menu.addAction(QStringLiteral("Show job log"));
    QAction *disabled = menu.addAction(QStringLiteral("Cancel job"));
    disabled->setEnabled(false);
    menu.popup(window->mapToGlobal(QPoint(20, 20)));
    QVERIFY(QTest::qWaitForWindowExposed(&menu));
    menu.setActiveAction(active);
    QCoreApplication::processEvents();
    const QImage menuImage = menu.grab().toImage();
    const QPalette mp = menu.palette();
    auto textPart = [](QRect r) { return r.adjusted(r.height(), 1, -2, -1); };
    check(QStringLiteral("QMenu item"), NormalText, mp.color(QPalette::WindowText), mp.color(QPalette::Window),
          menuImage, textPart(menu.actionGeometry(menu.actions().at(0))));
    check(QStringLiteral("QMenu selected item"), NormalText, QColor(), QColor(), menuImage,
          textPart(menu.actionGeometry(active)));
    check(QStringLiteral("QMenu disabled item (exception)"), Disabled, QColor(), QColor(), menuImage,
          textPart(menu.actionGeometry(disabled)));
    menu.hide();

    // Tool tip
    auto *label = window->findChild<QLabel *>(QStringLiteral("plainLabel"));
    QToolTip::showText(label->mapToGlobal(QPoint(5, 5)), QStringLiteral("Tool tip text of a job"), label,
                       QRect(), 60000);
    QWidget *tip = nullptr;
    QTRY_VERIFY((tip = [] {
        for (QWidget *w : QApplication::topLevelWidgets())
            if (w->inherits("QTipLabel") && w->isVisible())
                return w;
        return static_cast<QWidget *>(nullptr);
    }()) != nullptr);
    QCoreApplication::processEvents();
    const QImage tipImage = tip->grab().toImage();
    check(QStringLiteral("QToolTip"), NormalText, tip->palette().color(QPalette::ToolTipText),
          tip->palette().color(QPalette::ToolTipBase), tipImage, tip->rect().adjusted(2, 2, -2, -2));
    QToolTip::hideText();
}

// Widgets of the application that colour rows of their own
void BThemeContrastTest::checkAppWidgets(BMainWindow *mainWindow)
{
    // Job table: background colours by level and status, foreground from them
    auto *jobs = mainWindow->findChild<BJobWidget *>();
    QVERIFY(jobs);
    jobs->processJsonResponse(jobPage());
    QTableView *table = nullptr;
    for (QTableView *t : jobs->findChildren<QTableView *>()) {
        if (t->model() && t->model()->rowCount() >= 11 && t->isVisible()) {
            table = t;
            break;
        }
    }
    QVERIFY(table);
    QCoreApplication::processEvents();
    const int rows = table->model()->rowCount();
    table->selectRow(0);
    hover(table, table->visualRect(table->model()->index(1, 1)).center());
    const QImage image = table->viewport()->grab().toImage();
    const int columns = table->model()->columnCount();
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            const QModelIndex index = table->model()->index(r, c);
            const QRect cellRect = table->visualRect(index);
            if (table->isColumnHidden(c) || !cellRect.intersects(table->viewport()->rect())
                || index.data().toString().isEmpty())
                continue;
            // The name column of every row, and every cell that colours itself
            // (level and status chips, coloured rows)
            const QVariant fg = index.data(Qt::ForegroundRole);
            const QVariant bg = index.data(Qt::BackgroundRole);
            const bool chip = index.data(BStatus::ToneRole).isValid()
                              || index.data(BStatus::ChipColorRole).isValid();
            if (c != 1 && !fg.isValid() && !bg.isValid() && !chip)
                continue;
            QColor fgc, bgc;
            if (!chip && r > 1) {
                fgc = fg.isValid() ? fg.value<QBrush>().color() : table->palette().color(QPalette::Text);
                bgc = bg.isValid() ? bg.value<QBrush>().color()
                                   : table->palette().color(r % 2 ? QPalette::AlternateBase : QPalette::Base);
            }
            QString name = QStringLiteral("BJobWidget %1 \"%2\"")
                               .arg(table->model()->headerData(c, Qt::Horizontal).toString(),
                                    index.data().toString());
            if (r == 0)
                name += QStringLiteral(" (selected)");
            else if (r == 1)
                name += QStringLiteral(" (hovered)");
            check(name, NormalText, fgc, bgc, image, cellRect.adjusted(2, 2, -2, -2));
        }
    }

    // Status bar (connection state colour from the settings)
    QStatusBar *statusBar = mainWindow->statusBar();
    const QImage barImage = statusBar->grab().toImage();
    for (const char *name : {"connectionLabel", "statusLabel"}) {
        auto *l = statusBar->findChild<QLabel *>(QLatin1String(name));
        if (!l || !l->isVisible() || l->text().isEmpty())
            continue;
        check(QStringLiteral("status bar %1 \"%2\"").arg(QLatin1String(name), l->text()), NormalText,
              l->palette().color(QPalette::WindowText), statusBar->palette().color(QPalette::Window),
              barImage, QRect(l->mapTo(statusBar, QPoint(0, 0)), l->size()));
    }

    // Job log view: error, warning and success lines (P1-1)
    QDialog dialog;
    auto *layout = new QVBoxLayout(&dialog);
    auto *view = new BJobLogView(&dialog);
    auto *model = new BJobLogModel(&dialog);
    view->setModel(model);
    model->setLogLines(logLines());
    layout->addWidget(view);
    dialog.resize(700, 300);
    settle(&dialog);
    hover(view, view->visualRect(model->index(2, 0)).center());
    const QImage logImage = view->viewport()->grab().toImage();
    for (int r = 0; r < model->rowCount(); ++r) {
        const QModelIndex index = model->index(r, 0);
        const QVariant fg = index.data(Qt::ForegroundRole);
        const QVariant bg = index.data(Qt::BackgroundRole);
        const QColor fgc = fg.isValid() ? fg.value<QBrush>().color() : view->palette().color(QPalette::Text);
        const QColor bgc = bg.isValid() ? bg.value<QBrush>().color()
                                        : view->palette().color(r % 2 ? QPalette::AlternateBase : QPalette::Base);
        QRect rect = view->visualRect(index).adjusted(2, 1, -2, -1);
        rect.setWidth(qMin(rect.width(), 380));
        check(QStringLiteral("BJobLogView line %1%2").arg(r).arg(r == 2 ? QStringLiteral(" (hovered)") : QString()),
              NormalText, fgc, bgc, logImage, rect);
    }
}

void BThemeContrastTest::screenshots(BMainWindow *mainWindow, QWidget *gallery, const QString &theme)
{
    const QString dir = qEnvironmentVariable("ONESIMUS_THEME_SCREENSHOT_DIR");
    if (dir.isEmpty())
        return;
    QVERIFY(QDir().mkpath(dir));
    auto save = [&](QWidget *w, const QString &name) {
        QCoreApplication::processEvents();
        const QString file = QStringLiteral("%1/%2-%3.png").arg(dir, name, theme);
        QVERIFY2(w->grab().save(file), qPrintable(file));
    };

    auto *tabs = mainWindow->findChild<QTabWidget *>();
    QVERIFY(tabs);
    for (int i = 0; i < tabs->count(); ++i) {
        tabs->setCurrentIndex(i);
        QCoreApplication::processEvents();
        QString name = tabs->tabText(i).toLower();
        name.replace(QLatin1Char('/'), QLatin1Char('-')).remove(QLatin1Char('&'));
        save(mainWindow, QStringLiteral("main-%1").arg(name));
    }
    tabs->setCurrentIndex(0);
    save(gallery, QStringLiteral("gallery"));

    QJsonObject job{{"jobid", "4711"}, {"name", "BackupCatalog"}, {"client", "pdc-fd"},
                    {"type", "B"}, {"level", "F"}, {"jobstatus", "T"},
                    {"starttime", "2026-11-18 23:10:00"}, {"jobfiles", "1284"},
                    {"jobbytes", "734003200"}, {"fileset", "Catalog"}};
    {
        BJobDetailsDialog details(job, nullptr, nullptr, nullptr);
        if (auto *view = details.findChild<BJobLogView *>())
            if (auto *model = qobject_cast<BJobLogModel *>(view->model()))
                model->setLogLines(logLines());
        settle(&details);
        auto *detailTabs = details.findChild<QTabWidget *>();
        for (int i = 0; detailTabs && i < detailTabs->count(); ++i) {
            detailTabs->setCurrentIndex(i);
            save(&details, QStringLiteral("dlg-job-details-%1").arg(detailTabs->tabText(i).toLower().remove(QLatin1Char('&'))));
        }
    }
    {
        BSettingsDialog settings(nullptr);
        settle(&settings);
        save(&settings, QStringLiteral("dlg-settings"));
    }
    {
        BConnectionWizard wizard(nullptr, nullptr, nullptr);
        settle(&wizard);
        save(&wizard, QStringLiteral("dlg-connection-wizard"));
    }
    {
        BCertificateGenerator generator;
        settle(&generator);
        save(&generator, QStringLiteral("dlg-certificate-generator"));
    }
    {
        BPFXConverter converter;
        settle(&converter);
        save(&converter, QStringLiteral("dlg-pfx-converter"));
    }
}

void BThemeContrastTest::contrast()
{
    QFETCH(QString, setting);
    QFETCH(int, desktop);
    QFETCH(QString, expected);
    m_rows.clear();

    // A desktop whose colours do not match the theme: another style and a
    // palette of the opposite brightness (what a platform theme gives)
    QStyleHints *hints = QGuiApplication::styleHints();
    hints->setColorScheme(Qt::ColorScheme(desktop));
    if (hints->colorScheme() != Qt::ColorScheme(desktop) && setting == QLatin1String("system"))
        QSKIP("The platform cannot force a colour scheme (offscreen); the row system-unknown "
              "runs the same path, btheme_test checks the mapping of a forced scheme");
    qApp->setStyleSheet(QString());
    QApplication::setStyle(QStyleFactory::create(QStringLiteral("Windows")));
    // "unknown": a dark desktop palette under the light theme
    const bool desktopDark = Qt::ColorScheme(desktop) != Qt::ColorScheme::Light;
    QPalette foreign(desktopDark ? QColor(0x20, 0x20, 0x20) : QColor(0xf0, 0xf0, 0xf0));
    if (!desktopDark) {
        foreign.setColor(QPalette::Base, Qt::white);
        foreign.setColor(QPalette::Text, Qt::black);
    }
    QApplication::setPalette(foreign);

    BSettings::instance().setAppearanceTheme(setting);
    auto mainWindow = std::make_unique<BMainWindow>();
    mainWindow->resize(1200, 800);
    settle(mainWindow.get());
    QCOMPARE(QApplication::palette().color(QPalette::Base).lightness() < 128,
             expected == QLatin1String("dark"));

    // The widget gallery
    QDialog gallery;
    gallery.setWindowTitle(QStringLiteral("Widget gallery"));
    auto *layout = new QVBoxLayout(&gallery);
    auto *label = new QLabel(QStringLiteral("Plain label text"), &gallery);
    label->setObjectName(QStringLiteral("plainLabel"));
    layout->addWidget(label);
    auto *states = new QHBoxLayout;
    for (const char *state : {"success", "info", "warning", "error", "muted", "active"}) {
        auto *l = new QLabel(QStringLiteral("State %1").arg(QLatin1String(state)), &gallery);
        l->setObjectName(QStringLiteral("state-%1").arg(QLatin1String(state)));
        bSetLabelState(l, state);
        states->addWidget(l);
    }
    layout->addLayout(states);
    auto *edits = new QHBoxLayout;
    auto *edit = new QLineEdit(QStringLiteral("Line edit text"), &gallery);
    edit->setObjectName(QStringLiteral("edit"));
    auto *placeholder = new QLineEdit(&gallery);
    placeholder->setObjectName(QStringLiteral("placeholder"));
    placeholder->setPlaceholderText(QStringLiteral("Placeholder text"));
    auto *disabledEdit = new QLineEdit(QStringLiteral("Disabled text"), &gallery);
    disabledEdit->setObjectName(QStringLiteral("disabledEdit"));
    disabledEdit->setEnabled(false);
    auto *spin = new QSpinBox(&gallery);
    spin->setObjectName(QStringLiteral("spin"));
    spin->setRange(0, 99999);
    spin->setValue(4711);
    spin->setMinimumWidth(120);
    auto *combo = new QComboBox(&gallery);
    combo->setObjectName(QStringLiteral("combo"));
    combo->addItems({QStringLiteral("Full backup"), QStringLiteral("Incremental"),
                     QStringLiteral("Differential"), QStringLiteral("VirtualFull")});
    combo->setMinimumWidth(160);
    for (QWidget *w : {static_cast<QWidget *>(edit), static_cast<QWidget *>(placeholder),
                       static_cast<QWidget *>(disabledEdit), static_cast<QWidget *>(spin),
                       static_cast<QWidget *>(combo)})
        edits->addWidget(w);
    layout->addLayout(edits);

    auto *tabs = new QTabWidget(&gallery);
    tabs->setObjectName(QStringLiteral("tabs"));
    auto *page = new QWidget;
    auto *pageLayout = new QHBoxLayout(page);
    auto *table = new QTableWidget(5, 2, page);
    table->setObjectName(QStringLiteral("table"));
    table->setAlternatingRowColors(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setHorizontalHeaderLabels({QStringLiteral("Job name"), QStringLiteral("Client")});
    for (int r = 0; r < 5; ++r) {
        table->setItem(r, 0, new QTableWidgetItem(QStringLiteral("backup-job-%1").arg(r)));
        table->setItem(r, 1, new QTableWidgetItem(QStringLiteral("client-fd-%1").arg(r)));
    }
    table->setMouseTracking(true);
    table->setColumnWidth(0, 160);
    auto *tree = new QTreeWidget(page);
    tree->setObjectName(QStringLiteral("tree"));
    tree->setHeaderLabels({QStringLiteral("File")});
    tree->setMouseTracking(true);
    for (int i = 0; i < 3; ++i) {
        auto *top = new QTreeWidgetItem(tree, {QStringLiteral("/srv/data-%1").arg(i)});
        new QTreeWidgetItem(top, {QStringLiteral("file-%1.txt").arg(i)});
    }
    pageLayout->addWidget(table);
    pageLayout->addWidget(tree);
    tabs->addTab(page, QStringLiteral("Tables"));
    auto *other = new QWidget;
    auto *otherLayout = new QVBoxLayout(other);
    auto *plain = new QPlainTextEdit(QStringLiteral("Plain text edit\nsecond line"), other);
    plain->setObjectName(QStringLiteral("plainText"));
    otherLayout->addWidget(plain);
    tabs->addTab(other, QStringLiteral("Text"));
    layout->addWidget(tabs);

    auto *group = new QGroupBox(QStringLiteral("Group box title"), &gallery);
    group->setObjectName(QStringLiteral("group"));
    auto *groupLayout = new QHBoxLayout(group);
    auto *check1 = new QCheckBox(QStringLiteral("Check box text"), group);
    check1->setObjectName(QStringLiteral("check"));
    groupLayout->addWidget(check1);
    auto *button = new QPushButton(QStringLiteral("Push button"), group);
    button->setObjectName(QStringLiteral("button"));
    auto *defaultButton = new QPushButton(QStringLiteral("Default button"), group);
    defaultButton->setObjectName(QStringLiteral("defaultButton"));
    defaultButton->setDefault(true);
    auto *disabledButton = new QPushButton(QStringLiteral("Disabled button"), group);
    disabledButton->setObjectName(QStringLiteral("disabledButton"));
    disabledButton->setEnabled(false);
    groupLayout->addWidget(button);
    groupLayout->addWidget(defaultButton);
    groupLayout->addWidget(disabledButton);
    layout->addWidget(group);
    layout->addWidget(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &gallery));
    gallery.resize(900, 560);
    settle(&gallery);

    checkGallery(&gallery);
    checkItemViews(&gallery);
    checkPopups(&gallery);
    checkAppWidgets(mainWindow.get());

    // The palette itself (BTheme::palette()), for the roles the style sheet leaves out
    const QPalette app = QApplication::palette();
    checkPalette(QStringLiteral("palette Text/Base"), NormalText, app.color(QPalette::Text), app.color(QPalette::Base));
    checkPalette(QStringLiteral("palette Text/AlternateBase"), NormalText, app.color(QPalette::Text),
                 app.color(QPalette::AlternateBase));
    checkPalette(QStringLiteral("palette WindowText/Window"), NormalText, app.color(QPalette::WindowText),
                 app.color(QPalette::Window));
    checkPalette(QStringLiteral("palette ButtonText/Button"), NormalText, app.color(QPalette::ButtonText),
                 app.color(QPalette::Button));
    checkPalette(QStringLiteral("palette HighlightedText/Highlight"), NormalText,
                 app.color(QPalette::HighlightedText), app.color(QPalette::Highlight));
    checkPalette(QStringLiteral("palette ToolTipText/ToolTipBase"), NormalText,
                 app.color(QPalette::ToolTipText), app.color(QPalette::ToolTipBase));
    checkPalette(QStringLiteral("palette PlaceholderText/Base"), NormalText,
                 app.color(QPalette::PlaceholderText), app.color(QPalette::Base));
    checkPalette(QStringLiteral("palette Link/Base"), NormalText, app.color(QPalette::Link), app.color(QPalette::Base));

    screenshots(mainWindow.get(), &gallery, QString::fromLatin1(QTest::currentDataTag()));

    // The table: printed always, written next to the screenshots if asked
    QStringList table_;
    QStringList failures;
    table_ << QStringLiteral("%1 | %2 | %3 | %4 | %5 | %6")
                  .arg(QStringLiteral("widget"), -48).arg(QStringLiteral("palette"), 7)
                  .arg(QStringLiteral("render"), 7).arg(QStringLiteral("min"), 4)
                  .arg(QStringLiteral("ok"), 4).arg(QStringLiteral("colours"));
    for (const Row &row : std::as_const(m_rows)) {
        const bool paletteOk = row.palette == 0 || row.palette >= row.required;
        const bool renderOk = row.rendered == 0 || row.rendered >= row.required;
        const bool ok = paletteOk && renderOk;
        auto num = [](double v) { return v == 0 ? QStringLiteral("-") : QString::number(v, 'f', 2); };
        const QString line = QStringLiteral("%1 | %2 | %3 | %4 | %5 | %6")
                                 .arg(row.widget, -48).arg(num(row.palette), 7).arg(num(row.rendered), 7)
                                 .arg(row.required, 4, 'f', 1).arg(ok ? QStringLiteral("pass") : QStringLiteral("FAIL"), 4)
                                 .arg(row.note);
        table_ << line;
        if (!ok)
            failures << line;
    }
    for (const QString &line : std::as_const(table_))
        qInfo().noquote() << line;
    const QString dir = qEnvironmentVariable("ONESIMUS_THEME_SCREENSHOT_DIR");
    if (!dir.isEmpty()) {
        QFile out(QStringLiteral("%1/contrast-%2.txt").arg(dir, QString::fromLatin1(QTest::currentDataTag())));
        if (out.open(QIODevice::WriteOnly | QIODevice::Text))
            out.write(table_.join(QLatin1Char('\n')).toUtf8() + '\n');
    }
    QVERIFY2(failures.isEmpty(), qPrintable(QStringLiteral("\n") + failures.join(QLatin1Char('\n'))));
}

// Code names a meaning (bSetLabelState(), BStatus::textSpan(), the palette);
// a text colour fixed in a widget's style sheet fits one theme at best
void BThemeContrastTest::noFixedTextColours()
{
    static const QRegularExpression styleSheet(
        QStringLiteral("setStyleSheet\\((?:QStringLiteral\\()?\"([^\"]*)\""));
    static const QRegularExpression fixedColour(
        QStringLiteral("(?:^|[;{\\s])color\\s*:\\s*(?!palette\\()[#\\w]"));
    QStringList found;
    QDirIterator it(QStringLiteral(ONESIMUS_SOURCE_DIR "/src"), {QStringLiteral("*.cpp")},
                    QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFile file(it.next());
        QVERIFY(file.open(QIODevice::ReadOnly | QIODevice::Text));
        int number = 0;
        while (!file.atEnd()) {
            const QString line = QString::fromUtf8(file.readLine());
            ++number;
            const QRegularExpressionMatch m = styleSheet.match(line);
            if (!m.hasMatch())
                continue;
            const QString css = m.captured(1);
            // A fixed text colour together with its own background is readable
            if (fixedColour.match(css).hasMatch() && !css.contains(QLatin1String("background")))
                found << QStringLiteral("%1:%2: %3").arg(file.fileName()).arg(number).arg(line.trimmed());
        }
    }
    QVERIFY2(found.isEmpty(), qPrintable(QStringLiteral("\n") + found.join(QLatin1Char('\n'))));
}

QTEST_MAIN(BThemeContrastTest)
#include "bthemecontrast_test.moc"
