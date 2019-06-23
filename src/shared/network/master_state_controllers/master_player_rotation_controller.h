#ifndef SHARED_NETWORK_MASTER_PLAYER_ROTATION_CONTROLLER
#define SHARED_NETWORK_MASTER_PLAYER_ROTATION_CONTROLLER

#include "shared/network/network_base.h"
#include "shared/network/master_state_controllers/master_object_rotation_controller.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class MasterPlayerRotationController final : public MasterObjectRotationController
{
public:
	MasterPlayerRotationController(hermes::network::NetworkObjectStateId networkObjectStateId, float syncThreshold);
	hermes::network::NetworkObjectStateDataSPtr GetCurrentStateData() const override;
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_MASTER_PLAYER_ROTATION_CONTROLLER