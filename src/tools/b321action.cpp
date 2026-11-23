// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

// The Tools menu entry of the 3-2-1 verifier and its wiring to the Director.
// Apart from b321dialog.cpp so that the dialog and its test need no Director.

#include "tools/b321dialog.h"

#include <QAction>
#include <QPointer>

#include <memory>

QAction *B321Dialog::createAction(QWidget *parent, BDirector *director, ContextProvider context)
{
    auto *action = new QAction(tr("3-2-1 &Verifier..."), parent);
    action->setToolTip(tr("Check for every backup job: 3 copies, 2 media types, 1 copy off site "
                          "(read only)"));
    auto window = std::make_shared<QPointer<B321Dialog>>();

    QObject::connect(action, &QAction::triggered, parent, [parent, director, context, window]() {
        if (B321Dialog *open = window->data()) {
            open->show();
            open->raise();
            open->activateWindow();
            return;
        }
        auto *dialog = new B321Dialog(context, parent);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        *window = dialog;
        if (director) {
            // Out: read commands only (the dialog sends nothing else)
            QObject::connect(dialog, &B321Dialog::sendCommand, director,
                             [director](BDirector::Command cmd, const QString &args) {
                                 director->doSend(cmd, args);
                             });
            // In: the replies it asked for, and the volume list the Storage tab loads
            QObject::connect(director, &BDirector::jobHistoryResult, dialog, &B321Dialog::processJobHistory);
            QObject::connect(director, &BDirector::jsonResult, dialog, &B321Dialog::processJsonResult);
            QObject::connect(director, &BDirector::commandFailed, dialog, &B321Dialog::processCommandFailed);
            QObject::connect(director, &BDirector::connected, dialog, [dialog]() { dialog->setConnected(true); });
            QObject::connect(director, &BDirector::disconnected, dialog, [dialog]() { dialog->setConnected(false); });
            dialog->setConnected(director->isConnected());
        }
        dialog->show();
        dialog->refresh();
    });
    return action;
}
