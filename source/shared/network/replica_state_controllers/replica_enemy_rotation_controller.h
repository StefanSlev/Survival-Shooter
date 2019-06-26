#ifndef SHARED_NETWORK_REPLICA_ENEMY_ROTATION_CONTROLLER
#define SHARED_NETWORK_REPLICA_ENEMY_ROTATION_CONTROLLER

#include "shared/network/network_base.h"
#include "shared/network/replica_state_controllers/replica_object_rotation_controller.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class ReplicaEnemyRotationController final : public ReplicaObjectRotationController
{
public:
	ReplicaEnemyRotationController(hermes::network::NetworkObjectStateId networkObjectStateId, std::uint16_t maxNumOfStateData);

	hermes::network::NetworkObjectStateDataSPtr GetCurrentStateData() const override;
	void UpdateReplicaState() const override;
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_REPLICA_ENEMY_ROTATION_CONTROLLER