#include "shared/network/replica_state_controllers/replica_enemy_rotation_controller.h"
#include "shared/network/network_objects_state_data/network_object_rotation_data.h"
#include "shared/network/replica_objects/replica_enemy.h"
#include "shared/gameplay/engine/constants.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

ReplicaEnemyRotationController::ReplicaEnemyRotationController(hermes::network::NetworkObjectStateId networkObjectStateId, std::uint16_t maxNumOfStateData)
	: ReplicaObjectRotationController(networkObjectStateId, maxNumOfStateData)
{
}

hermes::network::NetworkObjectStateDataSPtr ReplicaEnemyRotationController::GetCurrentStateData() const
{
	hermes::network::ReplicaObject* replicaObject = this->GetReplicaObject();

	if (replicaObject)
	{
		auto replicaEnemy = static_cast<ReplicaEnemy*>(replicaObject);

		auto rotationData = std::make_shared<NetworkObjectRotationData>();
		rotationData->m_rotation = replicaEnemy->rotation;

		return std::static_pointer_cast<hermes::network::NetworkObjectStateData>(rotationData);
	}

	return nullptr;
}

void ReplicaEnemyRotationController::UpdateReplicaState() const
{
	auto lastStateData = this->GetLastStateData();

	if (lastStateData.size() > 0)
	{
		hermes::network::NetworkObjectStateDataSPtr stateData = lastStateData.back();
		auto rotationData = std::static_pointer_cast<NetworkObjectRotationData>(stateData);

		hermes::network::ReplicaObject* replicaObject = this->GetReplicaObject();

		if (replicaObject)
		{
			auto replicaEnemy = static_cast<ReplicaEnemy*>(replicaObject);
			gameplay::Vector2D currentLookDirection = gameplay::Vector2D(0, 1).Rotate(replicaEnemy->rotation);
			gameplay::Vector2D lookDirection = gameplay::Vector2D(0, 1).Rotate(rotationData->m_rotation);

			float dot = currentLookDirection.Dot(lookDirection);
			float det = currentLookDirection.Cross(lookDirection);
			float syncAngle = atan2f(det, dot) * 180.f / gameplay::M_PI;

			replicaEnemy->rotation += (syncAngle * 0.1f);
		}
	}
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE