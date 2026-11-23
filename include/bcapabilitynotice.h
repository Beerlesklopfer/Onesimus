// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BCAPABILITYNOTICE_H
#define BCAPABILITYNOTICE_H

#include "bcapabilities.h"

class BDirector;
class QWidget;

/**
 * @brief Message box for a function the connected Director does not provide
 *
 * Thin GUI part of BCapabilities: builds the server description from what
 * BDirector kept from the login and shows the notice. Call it only at the
 * moment the user asks for the function, never while connecting or
 * refreshing.
 *
 * The notice is additional information. It never replaces an error message
 * and callers continue exactly as they did without it.
 */
namespace BCapabilityNotice {

/** Server description of the Director behind @p director (Unknown if not connected). */
BCapabilities::Server serverOf(const BDirector *director);

/**
 * @brief Tell the user if @p function is not available on this Director
 *
 * Shows an information box with a "Do not show again" checkbox, subject to
 * BCapabilities::NoticePolicy (once per session for implicit requests, the
 * stored "Do not show again" for all). Nothing is shown without a
 * connection.
 *
 * @return true if the function is available on the Director (nothing shown);
 *         false if it is not, or if there is no connection
 */
bool check(QWidget *parent, const BDirector *director, BCapabilities::Function function,
           BCapabilities::Request request);

} // namespace BCapabilityNotice

#endif // BCAPABILITYNOTICE_H
