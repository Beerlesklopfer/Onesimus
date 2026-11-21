#ifndef BUSAGE_H
#define BUSAGE_H

#include <QMetaType>
#include <QString>
#include <QtGlobal>

/**
 * @file busage.h
 * @brief Free tier usage of the month and licence state of an Onesimus Director
 *
 * The Onesimus Director (odir) reports it typed, in proto/usage.proto
 * (package onesimus.director.usage): in DirectorStatusResponse.usage of
 * GetDirectorStatus and as the push event UsageEvent, which is sent right
 * after InteractiveSession starts and on every change. A classic Bareos
 * Director and BNET report nothing: then present is false.
 *
 * The GUI never computes usage from the catalog: these fields are the only
 * source.
 *
 * Header-only, free of protobuf: passed through the signals of the director
 * layer (BGrpcTransport -> BareosDirector -> BDirector -> BMainWindow).
 * BUsageProto converts the message.
 *
 * @author Joerg Bernau <support@onesimus.io>
 * @date 2026
 */
namespace BUsage {

/// UsageState of usage.proto
enum class State {
    Unspecified,
    Ok,         ///< below 80 % of the limit, or no limit
    Near,       ///< >= 80 %
    Reached     ///< >= 100 %: new backup jobs are held
};

/// LicenceKind of usage.proto
enum class Licence {
    Unspecified,
    Free,       ///< no licence file: free tier
    Licensed,   ///< valid signed licence file
    Expired,    ///< licence file expired: free tier applies
    Invalid     ///< signature, format or Director binding wrong: free tier applies
};

/// UsageInfo of usage.proto, with LicenceInfo flattened into it
struct Info {
    bool present = false;       ///< false: the Director reported nothing (BNET, Bareos, older odir)
    QString month;              ///< "2026-11", time zone of the Director
    quint64 bytesWritten = 0;   ///< finished backup jobs of the month
    quint64 limitBytes = 0;     ///< 0 = unlimited
    State state = State::Unspecified;   ///< computed by odir from written + running
    qint64 resetsAt = 0;        ///< unix time of the next month start, 0 = not known
    quint64 bytesRunning = 0;   ///< written so far by running counted jobs
    quint32 heldJobs = 0;       ///< scheduled jobs skipped this month because of the limit

    Licence licence = Licence::Unspecified;
    QString customer;
    QString licenceId;
    qint64 validUntil = 0;      ///< unix time, 0 = none
    QString licenceMessage;     ///< reason for Expired/Invalid

    /// Written by finished and running jobs: what the state is computed from
    quint64 used() const
    {
        const quint64 sum = bytesWritten + bytesRunning;
        return sum < bytesWritten ? ~quint64(0) : sum;
    }

    bool operator==(const Info &o) const
    {
        return present == o.present && month == o.month && bytesWritten == o.bytesWritten
            && limitBytes == o.limitBytes && state == o.state && resetsAt == o.resetsAt
            && bytesRunning == o.bytesRunning && heldJobs == o.heldJobs && licence == o.licence
            && customer == o.customer && licenceId == o.licenceId && validUntil == o.validUntil
            && licenceMessage == o.licenceMessage;
    }
    bool operator!=(const Info &o) const { return !(*this == o); }
};

} // namespace BUsage

Q_DECLARE_METATYPE(BUsage::Info)

#endif // BUSAGE_H
