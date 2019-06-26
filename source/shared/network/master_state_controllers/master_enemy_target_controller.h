#ifndef SHARED_NETWORK_MASTER_ENEMY_TARGET_CONTROLLER
#define SHARED_NETWORK_MASTER_ENEMY_TARGET_CONTROLLER

#include "shared/network/network_base.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class MasterEnemyTargetController final : public hermes::network::MasterStateController
{
public:
	MasterEnemyTargetController(hermes::network::NetworkObjectStateId networkObjectStateId, float syncThreshold);
	
	hermes::network::NetworkObjectStateDataSPtr GetCurrentStateData() const override;
	float ComputeSyncRate(hermes::network::NetworkObjectStateDataSPtr stateData) const override;
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_MASTER_ENEMY_TARGET_CONTROLLER