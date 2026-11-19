#ifndef BCONFIGCLIENT_H
#define BCONFIGCLIENT_H

#include <QObject>

#include <functional>

#include "director/bconfigtypes.h"

/**
 * @brief The configuration CRUD methods of the Director, behind an interface
 *
 * What the schedule views talk to when they read, create, change or delete
 * a resource. The application uses BDirectorConfigClient
 * (bdirectorconfigclient.h, over the gRPC
 * connection of BDirector); tests use a fake that records the requests and
 * answers them from a script, so that no test sends anything to a Director.
 *
 * Every call is answered exactly once through its callback, asynchronously,
 * also when there is no connection (Status::NotAvailable). A callback whose
 * context object is gone by then is not called.
 */
class BConfigClient : public QObject
{
    Q_OBJECT

public:
    using Callback = std::function<void(const BConfig::Reply &reply)>;

    explicit BConfigClient(QObject *parent = nullptr) : QObject(parent) {}

    /// What the Director lets this session do; known=false if not asked yet or on BNET
    virtual BConfig::Capabilities capabilities() const = 0;

    /// Sends @p request; @p callback runs with the reply while @p context lives
    virtual void call(const BConfig::Request &request, QObject *context, Callback callback) = 0;

signals:
    void capabilitiesChanged(const BConfig::Capabilities &capabilities);
};

#endif // BCONFIGCLIENT_H
