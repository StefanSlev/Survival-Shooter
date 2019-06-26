#ifndef SHARED_NETWORK_REPLICA_ENEMY_TARGET_CONTROLLER
#define SHARED_NETWORK_REPLICA_ENEMY_TARGET_CONTROLLER

#include "shared/network/network_base.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class ReplicaEnemyTargetController final : public hermes::network::ReplicaStateController
{
public:
	ReplicaEnemyTargetController(hermes::network::NetworkObjectStateId networkObjectStateId, std::uint16_t maxNumOfStateData);

	hermes::network::NetworkObjectStateDataSPtr ParseStateData(hermes::network::NetworkObjectStateSPtr objectState) const override;
	hermes::network::NetworkObjectStateDataSPtr GetCurrentStateData() const override;
	void UpdateReplicaState() const override;
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_REPLICA_ENEMY_TARGET_CONTROLLER