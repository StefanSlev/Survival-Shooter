#include "shared/network/replica_state_controllers/replica_enemy_position_controller.h"
#include "shared/network/network_objects_state_data/network_object_position_data.h"
#include "shared/network/replica_objects/replica_enemy.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

ReplicaEnemyPositionController::ReplicaEnemyPositionController(hermes::network::NetworkObjectStateId networkObjectStateId, std::uint16_t maxNumOfStateData)
	: ReplicaObjectPositionController(networkObjectStateId, maxNumOfStateData)
{
}

hermes::network::NetworkObjectStateDataSPtr ReplicaEnemyPositionController::GetCurrentStateData() const
{
	hermes::network::ReplicaObject* replicaObject = this->GetReplicaObject();

	if (replicaObject)
	{
		auto replicaEnemy = static_cast<ReplicaEnemy*>(replicaObject);

		auto positionData = std::make_shared<NetworkObjectPositionData>();
		positionData->m_position = replicaEnemy->position;

		return std::static_pointer_cast<hermes::network::NetworkObjectStateData>(positionData);
	}

	return nullptr;
}

void ReplicaEnemyPositionController::UpdateReplicaState() const
{
	auto lastStateData = this->GetLastStateData();

	if (lastStateData.size() > 0)
	{
		hermes::network::NetworkObjectStateDataSPtr stateData = lastStateData.back();
		auto positionData = std::static_pointer_cast<NetworkObjectPositionData>(stateData);

		hermes::network::ReplicaObject* replicaObject = this->GetReplicaObject();

		if (replicaObject)
		{
			auto replicaEnemy = static_cast<ReplicaEnemy*>(replicaObject);
			replicaEnemy->position = gameplay::Lerp(replicaEnemy->position, positionData->m_position, 0.1f);
		}
	}
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE