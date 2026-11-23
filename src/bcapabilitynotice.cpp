// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#include "bcapabilitynotice.h"

#include <QCheckBox>
#include <QCoreApplication>
#include <QMessageBox>

#include "config/bsettings.h"
#include "director/bdirector.h"

namespace BCapabilityNotice {

namespace {

// One policy for the whole application: the session memory is per process
BCapabilities::NoticePolicy &policy()
{
    static BCapabilities::NoticePolicy instance([](const QString &id) {
        return BSettings::instance().capabilityNoticeHidden(id);
    });
    return instance;
}

} // namespace

BCapabilities::Server serverOf(const BDirector *director)
{
    if (!director || !director->isConnected()) {
        return BCapabilities::Server();
    }
    const BCapabilities::Connection connection =
        director->transport() == BDirector::Transport::Grpc ? BCapabilities::Connection::Grpc
                                                            : BCapabilities::Connection::Classic;
    return BCapabilities::describeServer(connection, director->serverIdentity(),
                                         director->configCapabilities());
}

bool check(QWidget *parent, const BDirector *director, BCapabilities::Function function,
           BCapabilities::Request request)
{
    const BCapabilities::Entry *entry = BCapabilities::entry(function);
    if (!entry) {
        return true;
    }

    // Without a connection there is no Director to say anything about
    if (!director || !director->isConnected()) {
        return false;
    }

    const BCapabilities::Server server = serverOf(director);
    if (BCapabilities::isAvailable(*entry, server)) {
        return true;
    }
    if (!policy().shouldShow(*entry, request)) {
        return false;
    }
    // Recorded before the box opens, so that an implicit request arriving
    // while it is open does not put a second box on top
    policy().recordShown(*entry);

    const BCapabilities::Notice notice = BCapabilities::notice(*entry, server);

    QMessageBox box(QMessageBox::Information, notice.title, notice.text, QMessageBox::Ok, parent);
    auto *hideCheck = new QCheckBox(
        QCoreApplication::translate("BCapabilityNotice", "Do not show again"), &box);
    box.setCheckBox(hideCheck);
    box.exec();

    if (hideCheck->isChecked()) {
        BSettings::instance().setCapabilityNoticeHidden(QString::fromLatin1(entry->id), true);
    }
    return false;
}

} // namespace BCapabilityNotice
