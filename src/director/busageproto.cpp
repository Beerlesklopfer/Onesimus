// SPDX-License-Identifier: AGPL-3.0-or-later
// SPDX-FileCopyrightText: 2026 Jörg Bernau

/**
 * @file busageproto.cpp
 * @brief UsageInfo of proto/usage.proto as BUsage::Info
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */

#include "director/busageproto.h"

#include "usage.qpb.h"

namespace BUsageProto {

using namespace onesimus::director::usage;

BUsage::Info fromMessage(const UsageInfo &message)
{
    BUsage::Info info;
    info.present = true;
    info.month = message.month();
    info.bytesWritten = message.bytesWritten();
    info.limitBytes = message.limitBytes();
    info.resetsAt = message.resetsAt();
    info.bytesRunning = message.bytesRunning();
    info.heldJobs = message.heldJobsThisMonth();

    switch (message.state()) {
    case UsageStateGadget::UsageState::USAGE_STATE_OK:
        info.state = BUsage::State::Ok;
        break;
    case UsageStateGadget::UsageState::USAGE_STATE_NEAR:
        info.state = BUsage::State::Near;
        break;
    case UsageStateGadget::UsageState::USAGE_STATE_REACHED:
        info.state = BUsage::State::Reached;
        break;
    default:    // UNSPECIFIED or a value of a newer odir
        info.state = BUsage::State::Unspecified;
        break;
    }

    if (message.hasLicence()) {
        const LicenceInfo &licence = message.licence();
        switch (licence.kind()) {
        case LicenceKindGadget::LicenceKind::LICENCE_KIND_FREE:
            info.licence = BUsage::Licence::Free;
            break;
        case LicenceKindGadget::LicenceKind::LICENCE_KIND_LICENSED:
            info.licence = BUsage::Licence::Licensed;
            break;
        case LicenceKindGadget::LicenceKind::LICENCE_KIND_EXPIRED:
            info.licence = BUsage::Licence::Expired;
            break;
        case LicenceKindGadget::LicenceKind::LICENCE_KIND_INVALID:
            info.licence = BUsage::Licence::Invalid;
            break;
        default:
            info.licence = BUsage::Licence::Unspecified;
            break;
        }
        info.customer = licence.customer();
        info.licenceId = licence.licenceId();
        info.validUntil = licence.validUntil();
        info.licenceMessage = licence.message();
    }
    return info;
}

} // namespace BUsageProto
