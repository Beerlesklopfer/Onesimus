#ifndef BFAKECONFIGCLIENT_H
#define BFAKECONFIGCLIENT_H

// A BConfigClient for tests: a small Director in memory that follows the
// rules of odir's configuration CRUD (crates/odir/src/config_edit.rs):
// SHA-256 over the exact text, CONFLICT with the current text and SHA-256
// when expected_sha256 does not match (also when it is empty),
// validate_only writes nothing, REFERENCED for a schedule a job names,
// EXISTS for a name that is taken. Every request is recorded; intercept
// can answer a request with any reply instead. Nothing leaves the process.

#include <QHash>
#include <QPointer>
#include <QTimer>

#include <functional>
#include <optional>

#include "director/bconfigclient.h"

class BFakeConfigClient : public BConfigClient
{
    Q_OBJECT

public:
    explicit BFakeConfigClient(QObject *parent = nullptr) : BConfigClient(parent)
    {
        caps.known = true;
        caps.interfaceVersion = 1;
        caps.operations[int(BConfig::ResourceType::Schedule)] = {
            int(BConfig::Operation::ReadRaw), int(BConfig::Operation::CreateRaw),
            int(BConfig::Operation::UpdateRaw), int(BConfig::Operation::Delete)};
    }

    BConfig::Capabilities caps;
    QList<BConfig::Request> requests;
    QHash<QString, QString> files;                      ///< schedule name -> raw text
    QHash<QString, QList<BConfig::ResourceRef>> refs;   ///< schedule name -> who names it
    QStringList followups;                              ///< put into every successful write
    std::function<std::optional<BConfig::Reply>(const BConfig::Request &)> intercept;

    BConfig::Capabilities capabilities() const override { return caps; }

    void setCapabilities(const BConfig::Capabilities &capabilities)
    {
        caps = capabilities;
        emit capabilitiesChanged(caps);
    }

    void call(const BConfig::Request &request, QObject *context, Callback callback) override
    {
        requests.append(request);
        const BConfig::Reply reply = answer(request);
        QPointer<QObject> guard(context);
        QTimer::singleShot(0, this, [guard, callback, reply]() {
            if (guard && callback)
                callback(reply);
        });
    }

    /// The requests that would have changed the configuration
    QList<BConfig::Request> writes() const
    {
        QList<BConfig::Request> result;
        for (const BConfig::Request &request : requests) {
            if (request.writes())
                result.append(request);
        }
        return result;
    }

    static BConfig::Reply withText(BConfig::Status status, const QString &name, const QString &text)
    {
        BConfig::Reply reply;
        reply.status = status;
        reply.resource = BConfig::ResourceRef{BConfig::ResourceType::Schedule, name};
        reply.file = QStringLiteral("/etc/bareos/bareos-dir.d/schedule/%1.conf").arg(name);
        reply.text = text;
        reply.sha256 = text.isEmpty() ? QString() : BConfig::sha256Hex(text);
        return reply;
    }

private:
    BConfig::Reply answer(const BConfig::Request &request)
    {
        if (intercept) {
            if (const std::optional<BConfig::Reply> reply = intercept(request))
                return *reply;
        }
        const QString name = request.name;
        const QString current = files.value(name);
        switch (request.kind) {
        case BConfig::Request::Capabilities: {
            BConfig::Reply reply;
            reply.status = BConfig::Status::Ok;
            reply.interfaceVersion = caps.interfaceVersion;
            reply.operations = caps.operations;
            return reply;
        }
        case BConfig::Request::List: {
            BConfig::Reply reply;
            reply.status = BConfig::Status::Ok;
            for (auto it = files.cbegin(); it != files.cend(); ++it) {
                reply.resources.append(BConfig::Summary{
                    BConfig::ResourceRef{BConfig::ResourceType::Schedule, it.key()}, QString(),
                    BConfig::sha256Hex(it.value()), false});
            }
            return reply;
        }
        case BConfig::Request::Read:
            if (!files.contains(name))
                return withText(BConfig::Status::NotFound, name, QString());
            return withText(BConfig::Status::Ok, name, current);
        case BConfig::Request::Update: {
            if (!files.contains(name))
                return withText(BConfig::Status::NotFound, name, QString());
            if (request.expectedSha256.isEmpty() || request.expectedSha256 != BConfig::sha256Hex(current))
                return withText(BConfig::Status::Conflict, name, current);
            BConfig::Reply reply = withText(BConfig::Status::Ok, name, request.text);
            reply.validatedOnly = request.validateOnly;
            if (!request.validateOnly) {
                reply.oldSha256 = BConfig::sha256Hex(current);
                files.insert(name, request.text);
                reply.reloaded = true;
                reply.followups = followups;
            }
            return reply;
        }
        case BConfig::Request::Delete: {
            if (!files.contains(name))
                return withText(BConfig::Status::NotFound, name, QString());
            if (request.expectedSha256.isEmpty() || request.expectedSha256 != BConfig::sha256Hex(current))
                return withText(BConfig::Status::Conflict, name, current);
            if (!refs.value(name).isEmpty()) {
                BConfig::Reply reply = withText(BConfig::Status::Referenced, name, QString());
                reply.message = QStringLiteral("Schedule \"%1\" is used by %2 resources")
                                    .arg(name).arg(refs.value(name).size());
                reply.referencedBy = refs.value(name);
                return reply;
            }
            BConfig::Reply reply = withText(BConfig::Status::Ok, name, QString());
            reply.validatedOnly = request.validateOnly;
            if (!request.validateOnly) {
                reply.oldSha256 = BConfig::sha256Hex(current);
                files.remove(name);
                reply.reloaded = true;
            }
            return reply;
        }
        case BConfig::Request::Create: {
            // The name of the text
            const int at = request.text.indexOf(QStringLiteral("Name = \""));
            const int end = at >= 0 ? request.text.indexOf(QLatin1Char('"'), at + 8) : -1;
            const QString created = end > at ? request.text.mid(at + 8, end - at - 8) : QString();
            if (files.contains(created))
                return withText(BConfig::Status::Exists, created, QString());
            BConfig::Reply reply = withText(BConfig::Status::Ok, created, request.text);
            reply.validatedOnly = request.validateOnly;
            if (!request.validateOnly) {
                files.insert(created, request.text);
                reply.reloaded = true;
            }
            return reply;
        }
        }
        return BConfig::Reply();
    }
};

#endif // BFAKECONFIGCLIENT_H
