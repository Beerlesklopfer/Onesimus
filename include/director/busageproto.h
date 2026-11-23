// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

#ifndef BUSAGEPROTO_H
#define BUSAGEPROTO_H

#include "director/busage.h"

namespace onesimus::director::usage {
class UsageInfo;
}

/**
 * @file busageproto.h
 * @brief UsageInfo of proto/usage.proto as BUsage::Info
 *
 * Fields that are zero are absent on the wire (proto3) and read as zero.
 * Only built with gRPC.
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */
namespace BUsageProto {

/// The message as the director layer passes it on; present is true
BUsage::Info fromMessage(const onesimus::director::usage::UsageInfo &message);

} // namespace BUsageProto

#endif // BUSAGEPROTO_H
