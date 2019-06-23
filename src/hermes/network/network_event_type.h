#ifndef HERMES_NETWORK_EVENT_TYPE
#define HERMES_NETWORK_EVENT_TYPE

#include "hermes/network/network_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

enum class NetworkEventType : std::uint32_t
{
	NetworkFlow,
	NetworkFlowInitialize,
	CreateReplicaObject,
	DestroyReplicaObject
};

#define GetNetworkEventTypeIndex(networkEventType) static_cast<int> (networkEventType)

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_EVENT_TYPE