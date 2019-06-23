#ifndef SHARED_NETWORK_MASTER_ENEMY_POSITION_CONTROLLER
#define SHARED_NETWORK_MASTER_ENEMY_POSITION_CONTROLLER

#include "shared/network/network_base.h"
#include "shared/network/master_state_controllers/master_object_position_controller.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class MasterEnemyPositionController final : public MasterObjectPositionController
{
public:
	MasterEnemyPositionController(hermes::network::NetworkObjectStateId networkObjectStateId, float syncThreshold);
	hermes::network::NetworkObjectStateDataSPtr GetCurrentStateData() const final;
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_MASTER_ENEMY_POSITION_CONTROLLER