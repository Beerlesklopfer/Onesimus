#ifndef BCONFIGTYPES_H
#define BCONFIGTYPES_H

#include <QCryptographicHash>
#include <QHash>
#include <QList>
#include <QMetaType>
#include <QSet>
#include <QString>
#include <QStringList>

#include <optional>

/**
 * @file bconfigtypes.h
 * @brief Configuration resources as raw text: requests and replies of the
 *        CRUD methods of the Onesimus Director, without QtGrpc
 *
 * The Onesimus Director (odir) offers in its gRPC service DirectorApi the
 * methods GetConfigCapabilities, ListResources, ReadResource, CreateResource,
 * UpdateResource and DeleteResource (proto/director_api.proto; plan in
 * onesimus-dird docs/plans/CONFIG-CRUD-GRPC.md). They read and write the text
 * of one resource exactly as it stands in its file. These types mirror the
 * messages so that everything above the transport (BConfigClient, the
 * schedule views, the tests) is QtCore only. The conversion to and from the
 * generated messages is in bconfigproto.h; numbers of the enums are those of
 * the proto.
 *
 * Classic Bareos and BNET connections have none of this: every call is
 * answered with Status::NotAvailable there, without a word to the Director.
 */
namespace BConfig {

/// proto ResourceType
enum class ResourceType : int {
    Unspecified = 0,
    Schedule = 1,
    Job = 2,
    JobDefs = 3,
    Client = 4,
    FileSet = 5,
    Pool = 6,
    Storage = 7,
    Messages = 8,
    Catalog = 9,
    Console = 10,
    Profile = 11,
    Counter = 12,
    User = 13,
    Director = 14,
};

/// proto ConfigOperation
enum class Operation : int {
    Unspecified = 0,
    ReadRaw = 1,
    CreateRaw = 2,
    UpdateRaw = 3,
    Delete = 4,
    Rename = 5,
};

/**
 * @brief Outcome of a call
 *
 * Ok to ReloadFailed are the proto ConfigStatus values. The others are set
 * by the GUI: the call did not reach a status of the Director.
 */
enum class Status : int {
    Unspecified = 0,
    Ok = 1,
    Conflict = 2,       ///< expected_sha256 does not match; text/sha256 are the current state
    Invalid = 3,        ///< the new configuration is invalid; problem is set, nothing changed
    Referenced = 4,     ///< delete refused; referencedBy is set
    Denied = 5,         ///< resource ACL of the console
    NotFound = 6,
    Exists = 7,         ///< create: the name is taken
    ReloadFailed = 8,   ///< written, the reload failed, the old file was put back

    RpcError = 100,     ///< gRPC error (PERMISSION_DENIED, UNIMPLEMENTED, UNAVAILABLE ...): rpcCode, rpcMessage
    NotAvailable = 101, ///< no gRPC connection: nothing was sent
};

struct ResourceRef
{
    ResourceType type = ResourceType::Unspecified;
    QString name;

    bool operator==(const ResourceRef &other) const { return type == other.type && name == other.name; }
};

/// Where the new configuration is invalid
struct Problem
{
    QString file;
    int line = 0;       ///< 0: not known
    int column = 0;     ///< 0: not known
    QString text;       ///< as odir -t prints it
};

/// What a request asks for
struct Request
{
    enum Kind {
        Capabilities,   ///< GetConfigCapabilities
        List,           ///< ListResources(type)
        Read,           ///< ReadResource(type, name)
        Create,         ///< CreateResource(type, text, validateOnly)
        Update,         ///< UpdateResource(type, name, expectedSha256, text, validateOnly)
        Delete,         ///< DeleteResource(type, name, expectedSha256, validateOnly)
    };

    Kind kind = Read;
    ResourceType type = ResourceType::Unspecified;
    QString name;
    QString text;
    QString expectedSha256;
    bool validateOnly = false;

    static Request read(ResourceType type, const QString &name)
    {
        Request r;
        r.kind = Read;
        r.type = type;
        r.name = name;
        return r;
    }
    static Request create(ResourceType type, const QString &text, bool validateOnly)
    {
        Request r;
        r.kind = Create;
        r.type = type;
        r.text = text;
        r.validateOnly = validateOnly;
        return r;
    }
    static Request update(ResourceType type, const QString &name, const QString &expectedSha256,
                          const QString &text, bool validateOnly)
    {
        Request r;
        r.kind = Update;
        r.type = type;
        r.name = name;
        r.expectedSha256 = expectedSha256;
        r.text = text;
        r.validateOnly = validateOnly;
        return r;
    }
    static Request remove(ResourceType type, const QString &name, const QString &expectedSha256,
                          bool validateOnly)
    {
        Request r;
        r.kind = Delete;
        r.type = type;
        r.name = name;
        r.expectedSha256 = expectedSha256;
        r.validateOnly = validateOnly;
        return r;
    }

    /// Does the request change the configuration if it succeeds?
    bool writes() const { return (kind == Create || kind == Update || kind == Delete) && !validateOnly; }
};

/// One entry of ListResources
struct Summary
{
    ResourceRef resource;
    QString file;
    QString sha256;
    bool sharedFile = false;
};

/// The reply to any request (proto ResourceResponse, plus what the GUI adds)
struct Reply
{
    Status status = Status::Unspecified;
    int rpcCode = 0;            ///< gRPC status code (Status::RpcError)
    QString rpcMessage;         ///< "CODE: reason", readable (Status::RpcError, NotAvailable)

    QString message;            ///< the Director's readable message
    ResourceRef resource;
    QString file;
    QString text;               ///< raw text of the resource (current state)
    QString sha256;             ///< SHA-256 (hex) of text
    bool sharedFile = false;
    QList<ResourceRef> referencedBy;
    std::optional<Problem> problem;
    bool reloaded = false;
    bool validatedOnly = false;
    QString oldSha256;
    QStringList followups;

    // GetConfigCapabilities
    int interfaceVersion = 0;
    QHash<int, QSet<int>> operations;   ///< ResourceType -> Operations

    // ListResources
    QList<Summary> resources;

    bool ok() const { return status == Status::Ok; }
};

/**
 * @brief What the Director lets this session do (GetConfigCapabilities)
 *
 * Fetched once after the gRPC login. Unknown until then, and on BNET. A
 * Director without the method (UNIMPLEMENTED) is known to offer nothing.
 */
struct Capabilities
{
    bool known = false;
    int interfaceVersion = 0;
    QHash<int, QSet<int>> operations;   ///< ResourceType -> Operations

    bool supports(ResourceType type, Operation operation) const
    {
        return known && operations.value(int(type)).contains(int(operation));
    }

    bool operator==(const Capabilities &other) const
    {
        return known == other.known && interfaceVersion == other.interfaceVersion
            && operations == other.operations;
    }
    bool operator!=(const Capabilities &other) const { return !(*this == other); }
};

/// SHA-256 (lowercase hex) over the UTF-8 bytes of @p text, as odir computes it
inline QString sha256Hex(const QString &text)
{
    return QString::fromLatin1(QCryptographicHash::hash(text.toUtf8(), QCryptographicHash::Sha256).toHex());
}

/// "Schedule", "Job", ... as in the configuration files
inline QString typeName(ResourceType type)
{
    switch (type) {
    case ResourceType::Schedule: return QStringLiteral("Schedule");
    case ResourceType::Job: return QStringLiteral("Job");
    case ResourceType::JobDefs: return QStringLiteral("JobDefs");
    case ResourceType::Client: return QStringLiteral("Client");
    case ResourceType::FileSet: return QStringLiteral("FileSet");
    case ResourceType::Pool: return QStringLiteral("Pool");
    case ResourceType::Storage: return QStringLiteral("Storage");
    case ResourceType::Messages: return QStringLiteral("Messages");
    case ResourceType::Catalog: return QStringLiteral("Catalog");
    case ResourceType::Console: return QStringLiteral("Console");
    case ResourceType::Profile: return QStringLiteral("Profile");
    case ResourceType::Counter: return QStringLiteral("Counter");
    case ResourceType::User: return QStringLiteral("User");
    case ResourceType::Director: return QStringLiteral("Director");
    case ResourceType::Unspecified: break;
    }
    return QStringLiteral("Resource");
}

/// "OK", "CONFLICT", ... for logs and tests
inline QString statusName(Status status)
{
    switch (status) {
    case Status::Unspecified: return QStringLiteral("UNSPECIFIED");
    case Status::Ok: return QStringLiteral("OK");
    case Status::Conflict: return QStringLiteral("CONFLICT");
    case Status::Invalid: return QStringLiteral("INVALID");
    case Status::Referenced: return QStringLiteral("REFERENCED");
    case Status::Denied: return QStringLiteral("DENIED");
    case Status::NotFound: return QStringLiteral("NOT_FOUND");
    case Status::Exists: return QStringLiteral("EXISTS");
    case Status::ReloadFailed: return QStringLiteral("RELOAD_FAILED");
    case Status::RpcError: return QStringLiteral("RPC_ERROR");
    case Status::NotAvailable: return QStringLiteral("NOT_AVAILABLE");
    }
    return QStringLiteral("UNSPECIFIED");
}

} // namespace BConfig

Q_DECLARE_METATYPE(BConfig::Request)
Q_DECLARE_METATYPE(BConfig::Reply)
Q_DECLARE_METATYPE(BConfig::Capabilities)

#endif // BCONFIGTYPES_H
