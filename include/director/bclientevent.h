// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BCLIENTEVENT_H
#define BCLIENTEVENT_H

/**
 * @brief Client events the Onesimus Director pushes on its gRPC session
 *
 * From PushEvent fd_connected, fd_disconnected and fd_status
 * (director_service.proto). The signals of the director classes carry the
 * value as int, so they cross the thread of the Director without a
 * registered meta type.
 */
enum class BClientEvent : int {
    Connected = 0,      ///< FdConnected: the Director reached the file daemon
    Disconnected = 1,   ///< FdDisconnected: that connection ended (after a job)
    Alive = 2,          ///< FdStatus alive=true
    Unreachable = 3     ///< FdStatus alive=false: the Director could not reach it
};

/**
 * @brief How a client probe ("Check now", status client=<name>) ended
 *
 * Carried as int by BareosDirector::clientProbeFinished() and
 * BDirector::clientProbeFinished(), like BClientEvent.
 */
enum class BClientProbeStatus : int {
    Replied = 0,        ///< The Director answered; the text says what it found
    Timeout = 1,        ///< No answer within the deadline
    Error = 2,          ///< The call failed (rights, unknown client, connection)
    NotAvailable = 3,   ///< This connection cannot carry the probe (console protocol)
    Busy = 4            ///< Another probe is still running; nothing was sent
};

#endif // BCLIENTEVENT_H
