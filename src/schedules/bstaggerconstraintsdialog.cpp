#include "schedules/bstaggerconstraintsdialog.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QTimeEdit>
#include <QVBoxLayout>

#include "buiscale.h"

BStaggerConstraintsDialog::BStaggerConstraintsDialog(
    const QStringList &jobs, const QHash<QString, BSettings::StaggerConstraint> &current,
    QWidget *parent)
    : QDialog(parent), m_current(current), m_jobs(jobs)
{
    setWindowTitle(tr("Constraints for the proposal"));
    auto *layout = new QVBoxLayout(this);

    auto *intro = new QLabel(tr("The proposal only starts jobs later. A fixed job is never moved, "
                                "and a job of a higher priority is then not moved past it either. "
                                "A job with a latest start does not start after that time. The "
                                "constraints are kept on this computer, for this Director."), this);
    intro->setWordWrap(true);
    layout->addWidget(intro);

    auto *area = new QScrollArea(this);
    area->setWidgetResizable(true);
    area->setFrameShape(QFrame::NoFrame);
    auto *inner = new QWidget(area);
    auto *grid = new QGridLayout(inner);
    grid->addWidget(new QLabel(tr("<b>Job</b>"), inner), 0, 0);
    grid->addWidget(new QLabel(tr("<b>Fixed</b>"), inner), 0, 1);
    grid->addWidget(new QLabel(tr("<b>Latest start</b>"), inner), 0, 2, 1, 2);

    if (jobs.isEmpty()) {
        grid->addWidget(new QLabel(tr("No job waits on the dates on display."), inner), 1, 0, 1, 4);
    }
    for (int i = 0; i < jobs.size(); ++i) {
        const QString &job = jobs[i];
        const BSettings::StaggerConstraint c = current.value(job);
        Row row;
        auto *name = new QLabel(job, inner);
        name->setTextFormat(Qt::PlainText);
        row.fixed = new QCheckBox(inner);
        row.fixed->setObjectName(QStringLiteral("fixed:") + job);
        row.fixed->setAccessibleName(tr("%1 is fixed").arg(job));
        row.fixed->setChecked(c.fixed);
        row.latestOn = new QCheckBox(tr("not after"), inner);
        row.latestOn->setObjectName(QStringLiteral("latestOn:") + job);
        row.latestOn->setAccessibleName(tr("%1 has a latest start").arg(job));
        row.latestOn->setChecked(c.latestStart.isValid());
        row.latest = new QTimeEdit(inner);
        row.latest->setObjectName(QStringLiteral("latest:") + job);
        row.latest->setDisplayFormat(QStringLiteral("HH:mm"));
        row.latest->setAccessibleName(tr("Latest start of %1").arg(job));
        row.latest->setTime(c.latestStart.isValid() ? c.latestStart : QTime(23, 59));
        name->setBuddy(row.fixed);

        // A fixed job does not move at all: its latest start says nothing
        auto sync = [row]() {
            row.latestOn->setEnabled(!row.fixed->isChecked());
            row.latest->setEnabled(!row.fixed->isChecked() && row.latestOn->isChecked());
        };
        connect(row.fixed, &QCheckBox::toggled, this, sync);
        connect(row.latestOn, &QCheckBox::toggled, this, sync);
        sync();

        grid->addWidget(name, i + 1, 0);
        grid->addWidget(row.fixed, i + 1, 1, Qt::AlignCenter);
        grid->addWidget(row.latestOn, i + 1, 2);
        grid->addWidget(row.latest, i + 1, 3);
        m_rows.insert(job, row);
    }
    grid->setColumnStretch(0, 1);
    grid->setRowStretch(jobs.size() + 1, 1);
    area->setWidget(inner);
    area->setMinimumSize(BUiScale::px(420), BUiScale::px(220));
    layout->addWidget(area, 1);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addWidget(buttons);
}

QHash<QString, BSettings::StaggerConstraint> BStaggerConstraintsDialog::constraints() const
{
    QHash<QString, BSettings::StaggerConstraint> out = m_current;
    for (const QString &job : m_jobs) {
        const Row row = m_rows.value(job);
        if (!row.fixed) continue;
        BSettings::StaggerConstraint c;
        c.fixed = row.fixed->isChecked();
        // A latest start beside "fixed" is kept as entered, but not used
        if (row.latestOn->isChecked())
            c.latestStart = QTime(row.latest->time().hour(), row.latest->time().minute());
        if (c.isEmpty())
            out.remove(job);
        else
            out.insert(job, c);
    }
    return out;
}
