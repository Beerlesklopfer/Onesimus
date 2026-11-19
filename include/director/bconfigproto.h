#ifndef BCONFIGPROTO_H
#define BCONFIGPROTO_H

#include <QByteArray>

#include "director/bconfigtypes.h"

#include "director_api.qpb.h"

/**
 * @file bconfigproto.h
 * @brief BConfig types to and from the generated messages of
 *        proto/director_api.proto (needs QtProtobuf; HAVE_GRPC builds only)
 *
 * The transport calls these; the tests use them to look at the bytes a
 * request becomes on the wire.
 */
namespace BConfigProto {

namespace pb = onesimus::director::api;

// --- Requests ----------------------------------------------------------------

pb::ConfigCapabilitiesRequest toCapabilitiesRequest(const BConfig::Request &request);
pb::ListResourcesRequest toListRequest(const BConfig::Request &request);
pb::ReadResourceRequest toReadRequest(const BConfig::Request &request);
pb::CreateResourceRequest toCreateRequest(const BConfig::Request &request);
pb::UpdateResourceRequest toUpdateRequest(const BConfig::Request &request);
pb::DeleteResourceRequest toDeleteRequest(const BConfig::Request &request);

// --- Replies -----------------------------------------------------------------

BConfig::Reply fromMessage(const pb::ResourceResponse &response);
BConfig::Reply fromMessage(const pb::ConfigCapabilitiesResponse &response);
BConfig::Reply fromMessage(const pb::ListResourcesResponse &response);
pb::ResourceResponse toMessage(const BConfig::Reply &reply);

/**
 * @brief The request as the serialized proto message of its RPC
 *
 * GetConfigCapabilities: ConfigCapabilitiesRequest, ListResources:
 * ListResourcesRequest, ReadResource: ReadResourceRequest, and so on.
 * The text field is carried as it is (UTF-8, line ends untouched).
 */
QByteArray serializeRequest(const BConfig::Request &request);

/**
 * @brief Reads serialized request bytes back (tests)
 * @param kind which message the bytes are
 * @return false if the bytes are not such a message
 */
bool parseRequest(BConfig::Request::Kind kind, const QByteArray &bytes, BConfig::Request *request);

/// A serialized ResourceResponse as a Reply
bool parseResourceResponse(const QByteArray &bytes, BConfig::Reply *reply);
/// A serialized ConfigCapabilitiesResponse as a Reply (interfaceVersion, operations)
bool parseCapabilitiesResponse(const QByteArray &bytes, BConfig::Reply *reply);
/// A serialized ListResourcesResponse as a Reply (resources)
bool parseListResponse(const QByteArray &bytes, BConfig::Reply *reply);

/// A Reply as a serialized ResourceResponse (tests: replies of a fake server)
QByteArray serializeResourceResponse(const BConfig::Reply &reply);

/// The capabilities of a capabilities reply
BConfig::Capabilities capabilitiesOf(const BConfig::Reply &reply);

} // namespace BConfigProto

#endif // BCONFIGPROTO_H
