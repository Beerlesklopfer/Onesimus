/**
 * @file bconfigproto.cpp
 * @brief BConfig types to and from the generated messages of director_api.proto
 */

#include "director/bconfigproto.h"

#include <QProtobufSerializer>
#include <QtProtobuf/qprotobufregistration.h>

#include <mutex>

namespace BConfigProto {

namespace {

pb::ResourceTypeGadget::ResourceType toType(BConfig::ResourceType type)
{
    return pb::ResourceTypeGadget::ResourceType(int(type));
}

BConfig::ResourceType fromType(pb::ResourceTypeGadget::ResourceType type)
{
    const int value = int(type);
    return (value >= 0 && value <= int(BConfig::ResourceType::Director)) ? BConfig::ResourceType(value)
                                                                         : BConfig::ResourceType::Unspecified;
}

pb::ResourceRef toRef(BConfig::ResourceType type, const QString &name)
{
    pb::ResourceRef ref;
    ref.setType(toType(type));
    ref.setName(name);
    return ref;
}

BConfig::ResourceRef fromRef(const pb::ResourceRef &ref)
{
    return BConfig::ResourceRef{fromType(ref.type()), ref.name()};
}

BConfig::Status fromStatus(pb::ConfigStatusGadget::ConfigStatus status)
{
    const int value = int(status);
    return (value >= int(BConfig::Status::Ok) && value <= int(BConfig::Status::ReloadFailed))
        ? BConfig::Status(value)
        : BConfig::Status::Unspecified;
}

template <typename Message>
QByteArray serialized(const Message &message)
{
    BConfigProto::registerTypes();
    QProtobufSerializer serializer;
    return message.serialize(&serializer);
}

template <typename Message>
bool parsed(const QByteArray &bytes, Message *message)
{
    BConfigProto::registerTypes();
    QProtobufSerializer serializer;
    return message->deserialize(&serializer, bytes);
}

} // namespace

void registerTypes()
{
    static std::once_flag once;
    std::call_once(once, [] {
        pb::ResourceTypeGadget::registerTypes();
        pb::ConfigOperationGadget::registerTypes();
        pb::ConfigStatusGadget::registerTypes();
        qRegisterProtobufType<pb::ResourceRef>();
        qRegisterProtobufType<pb::TypeCapabilities>();
        qRegisterProtobufType<pb::ConfigCapabilitiesRequest>();
        qRegisterProtobufType<pb::ConfigCapabilitiesResponse>();
        qRegisterProtobufType<pb::ListResourcesRequest>();
        qRegisterProtobufType<pb::ListResourcesResponse>();
        qRegisterProtobufType<pb::ResourceSummary>();
        qRegisterProtobufType<pb::ReadResourceRequest>();
        qRegisterProtobufType<pb::CreateResourceRequest>();
        qRegisterProtobufType<pb::UpdateResourceRequest>();
        qRegisterProtobufType<pb::DeleteResourceRequest>();
        qRegisterProtobufType<pb::ResourceResponse>();
        qRegisterProtobufType<pb::ConfigProblem>();
    });
}

// --- Requests ----------------------------------------------------------------

pb::ConfigCapabilitiesRequest toCapabilitiesRequest(const BConfig::Request &)
{
    return pb::ConfigCapabilitiesRequest();
}

pb::ListResourcesRequest toListRequest(const BConfig::Request &request)
{
    pb::ListResourcesRequest message;
    message.setType(toType(request.type));
    return message;
}

pb::ReadResourceRequest toReadRequest(const BConfig::Request &request)
{
    pb::ReadResourceRequest message;
    message.setResource(toRef(request.type, request.name));
    return message;
}

pb::CreateResourceRequest toCreateRequest(const BConfig::Request &request)
{
    pb::CreateResourceRequest message;
    message.setType(toType(request.type));
    message.setText(request.text);
    message.setValidateOnly(request.validateOnly);
    return message;
}

pb::UpdateResourceRequest toUpdateRequest(const BConfig::Request &request)
{
    pb::UpdateResourceRequest message;
    message.setResource(toRef(request.type, request.name));
    message.setExpectedSha256(request.expectedSha256);
    message.setText(request.text);
    message.setValidateOnly(request.validateOnly);
    return message;
}

pb::DeleteResourceRequest toDeleteRequest(const BConfig::Request &request)
{
    pb::DeleteResourceRequest message;
    message.setResource(toRef(request.type, request.name));
    message.setExpectedSha256(request.expectedSha256);
    message.setValidateOnly(request.validateOnly);
    return message;
}

QByteArray serializeRequest(const BConfig::Request &request)
{
    switch (request.kind) {
    case BConfig::Request::Capabilities: return serialized(toCapabilitiesRequest(request));
    case BConfig::Request::List: return serialized(toListRequest(request));
    case BConfig::Request::Read: return serialized(toReadRequest(request));
    case BConfig::Request::Create: return serialized(toCreateRequest(request));
    case BConfig::Request::Update: return serialized(toUpdateRequest(request));
    case BConfig::Request::Delete: return serialized(toDeleteRequest(request));
    }
    return QByteArray();
}

bool parseRequest(BConfig::Request::Kind kind, const QByteArray &bytes, BConfig::Request *request)
{
    BConfig::Request result;
    result.kind = kind;
    switch (kind) {
    case BConfig::Request::Capabilities: {
        pb::ConfigCapabilitiesRequest message;
        if (!parsed(bytes, &message))
            return false;
        break;
    }
    case BConfig::Request::List: {
        pb::ListResourcesRequest message;
        if (!parsed(bytes, &message))
            return false;
        result.type = fromType(message.type());
        break;
    }
    case BConfig::Request::Read: {
        pb::ReadResourceRequest message;
        if (!parsed(bytes, &message))
            return false;
        result.type = fromType(message.resource().type());
        result.name = message.resource().name();
        break;
    }
    case BConfig::Request::Create: {
        pb::CreateResourceRequest message;
        if (!parsed(bytes, &message))
            return false;
        result.type = fromType(message.type());
        result.text = message.text();
        result.validateOnly = message.validateOnly();
        break;
    }
    case BConfig::Request::Update: {
        pb::UpdateResourceRequest message;
        if (!parsed(bytes, &message))
            return false;
        result.type = fromType(message.resource().type());
        result.name = message.resource().name();
        result.expectedSha256 = message.expectedSha256();
        result.text = message.text();
        result.validateOnly = message.validateOnly();
        break;
    }
    case BConfig::Request::Delete: {
        pb::DeleteResourceRequest message;
        if (!parsed(bytes, &message))
            return false;
        result.type = fromType(message.resource().type());
        result.name = message.resource().name();
        result.expectedSha256 = message.expectedSha256();
        result.validateOnly = message.validateOnly();
        break;
    }
    }
    if (request)
        *request = result;
    return true;
}

// --- Replies -----------------------------------------------------------------

BConfig::Reply fromMessage(const pb::ResourceResponse &response)
{
    BConfig::Reply reply;
    reply.status = fromStatus(response.status());
    reply.message = response.message();
    if (response.hasResource())
        reply.resource = fromRef(response.resource());
    reply.file = response.file();
    reply.text = response.text();
    reply.sha256 = response.sha256();
    reply.sharedFile = response.sharedFile();
    for (const pb::ResourceRef &ref : response.referencedBy())
        reply.referencedBy.append(fromRef(ref));
    if (response.hasProblem()) {
        const pb::ConfigProblem &problem = response.problem();
        reply.problem = BConfig::Problem{problem.file(), int(problem.line()), int(problem.column()),
                                         problem.text()};
    }
    reply.reloaded = response.reloaded();
    reply.validatedOnly = response.validatedOnly();
    reply.oldSha256 = response.oldSha256();
    reply.followups = response.followups();
    return reply;
}

BConfig::Reply fromMessage(const pb::ConfigCapabilitiesResponse &response)
{
    BConfig::Reply reply;
    reply.status = BConfig::Status::Ok;
    reply.interfaceVersion = int(response.interfaceVersion());
    for (const pb::TypeCapabilities &type : response.types()) {
        QSet<int> &operations = reply.operations[int(type.type())];
        for (const pb::ConfigOperationGadget::ConfigOperation operation : type.operations())
            operations.insert(int(operation));
    }
    return reply;
}

BConfig::Reply fromMessage(const pb::ListResourcesResponse &response)
{
    BConfig::Reply reply;
    reply.status = BConfig::Status::Ok;
    for (const pb::ResourceSummary &summary : response.resources()) {
        BConfig::Summary entry;
        if (summary.hasResource())
            entry.resource = fromRef(summary.resource());
        entry.file = summary.file();
        entry.sha256 = summary.sha256();
        entry.sharedFile = summary.sharedFile();
        reply.resources.append(entry);
    }
    return reply;
}

pb::ResourceResponse toMessage(const BConfig::Reply &reply)
{
    pb::ResourceResponse message;
    const int status = int(reply.status);
    message.setStatus(status <= int(BConfig::Status::ReloadFailed)
                          ? pb::ConfigStatusGadget::ConfigStatus(status)
                          : pb::ConfigStatusGadget::ConfigStatus::CONFIG_STATUS_UNSPECIFIED);
    message.setMessage(reply.message);
    message.setResource(toRef(reply.resource.type, reply.resource.name));
    message.setFile(reply.file);
    message.setText(reply.text);
    message.setSha256(reply.sha256);
    message.setSharedFile(reply.sharedFile);
    QList<pb::ResourceRef> refs;
    for (const BConfig::ResourceRef &ref : reply.referencedBy)
        refs.append(toRef(ref.type, ref.name));
    message.setReferencedBy(refs);
    if (reply.problem) {
        pb::ConfigProblem problem;
        problem.setFile(reply.problem->file);
        problem.setLine(QtProtobuf::uint32(reply.problem->line));
        problem.setColumn(QtProtobuf::uint32(reply.problem->column));
        problem.setText(reply.problem->text);
        message.setProblem(problem);
    }
    message.setReloaded(reply.reloaded);
    message.setValidatedOnly(reply.validatedOnly);
    message.setOldSha256(reply.oldSha256);
    message.setFollowups(reply.followups);
    return message;
}

bool parseResourceResponse(const QByteArray &bytes, BConfig::Reply *reply)
{
    pb::ResourceResponse message;
    if (!parsed(bytes, &message))
        return false;
    if (reply)
        *reply = fromMessage(message);
    return true;
}

bool parseCapabilitiesResponse(const QByteArray &bytes, BConfig::Reply *reply)
{
    pb::ConfigCapabilitiesResponse message;
    if (!parsed(bytes, &message))
        return false;
    if (reply)
        *reply = fromMessage(message);
    return true;
}

bool parseListResponse(const QByteArray &bytes, BConfig::Reply *reply)
{
    pb::ListResourcesResponse message;
    if (!parsed(bytes, &message))
        return false;
    if (reply)
        *reply = fromMessage(message);
    return true;
}

QByteArray serializeResourceResponse(const BConfig::Reply &reply)
{
    return serialized(toMessage(reply));
}

BConfig::Capabilities capabilitiesOf(const BConfig::Reply &reply)
{
    BConfig::Capabilities capabilities;
    capabilities.known = true;
    capabilities.interfaceVersion = reply.interfaceVersion;
    capabilities.operations = reply.operations;
    return capabilities;
}

} // namespace BConfigProto
