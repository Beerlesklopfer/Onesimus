#ifndef BDIRECTORCONFIGCLIENT_H
#define BDIRECTORCONFIGCLIENT_H

#include <QHash>
#include <QPointer>

#include "director/bconfigclient.h"

class BDirector;

/**
 * @brief BConfigClient over BDirector (the worker thread and its gRPC transport)
 */
class BDirectorConfigClient : public BConfigClient
{
    Q_OBJECT

public:
    explicit BDirectorConfigClient(BDirector *director, QObject *parent = nullptr);

    BConfig::Capabilities capabilities() const override;
    void call(const BConfig::Request &request, QObject *context, Callback callback) override;

private:
    struct Pending
    {
        QPointer<QObject> context;
        Callback callback;
    };

    void onReply(quint64 id, const BConfig::Reply &reply);

    QPointer<BDirector> m_director;
    QHash<quint64, Pending> m_pending;
};

#endif // BDIRECTORCONFIGCLIENT_H
