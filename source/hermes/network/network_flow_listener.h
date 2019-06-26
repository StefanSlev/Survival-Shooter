#ifndef HERMES_NETWORK_FLOW_LISTENER
#define HERMES_NETWORK_FLOW_LISTENER

#include "hermes/network/network_base.h"
#include "hermes/network/network_object_id.h"
#include "hermes/network/network_event.h"
#include "hermes/network/replica_object.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class NetworkFlowListener
{
public:
	virtual void OnNetworkEvent(NetworkEventSPtr networkEvent) = 0;
	virtual void OnReplicaObjectCreated(std::uint32_t objectType, ReplicaObjectSPtr replicaObject) = 0;
	virtual void OnReplicaObjectDestroyed(NetworkObjectIdValue objectIdValue) = 0;
	virtual ~NetworkFlowListener();
};

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_FLOW_LISTENER