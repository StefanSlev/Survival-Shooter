#ifndef SHARED_NETWORK_MASTER_OBJECT_ROTATION_CONTROLLER
#define SHARED_NETWORK_MASTER_OBJECT_ROTATION_CONTROLLER

#include "shared/network/network_base.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class MasterObjectRotationController : public hermes::network::MasterStateController
{
public:
	MasterObjectRotationController(hermes::network::NetworkObjectStateId networkObjectStateId, float syncThreshold);
	float ComputeSyncRate(hermes::network::NetworkObjectStateDataSPtr stateData) const final;
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_MASTER_OBJECT_ROTATION_CONTROLLER