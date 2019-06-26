#ifndef SHARED_NETWORK_REPLICA_OBJECT_ROTATION_CONTROLLER
#define SHARED_NETWORK_REPLICA_OBJECT_ROTATION_CONTROLLER

#include "shared/network/network_base.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class ReplicaObjectRotationController : public hermes::network::ReplicaStateController
{
public:
	ReplicaObjectRotationController(hermes::network::NetworkObjectStateId networkObjectStateId, std::uint16_t maxNumOfStateData);
	hermes::network::NetworkObjectStateDataSPtr ParseStateData(hermes::network::NetworkObjectStateSPtr objectState) const final;
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_REPLICA_OBJECT_ROTATION_CONTROLLER