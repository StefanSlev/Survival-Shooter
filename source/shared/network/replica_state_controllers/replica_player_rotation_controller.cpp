#include "shared/network/replica_state_controllers/replica_player_rotation_controller.h"
#include "shared/network/network_objects_state_data/network_object_rotation_data.h"
#include "shared/network/replica_objects/replica_player.h"
#include "shared/gameplay/engine/constants.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

ReplicaPlayerRotationController::ReplicaPlayerRotationController(hermes::network::NetworkObjectStateId networkObjectStateId, std::uint16_t maxNumOfStateData)
	: ReplicaObjectRotationController(networkObjectStateId, maxNumOfStateData)
{
}

hermes::network::NetworkObjectStateDataSPtr ReplicaPlayerRotationController::GetCurrentStateData() const
{
	hermes::network::ReplicaObject* replicaObject = this->GetReplicaObject();

	if (replicaObject)
	{
		auto replicaPlayer = static_cast<ReplicaPlayer*>(replicaObject);

		auto rotationData = std::make_shared<NetworkObjectRotationData>();
		rotationData->m_rotation = replicaPlayer->rotation;

		return std::static_pointer_cast<hermes::network::NetworkObjectStateData>(rotationData);
	}

	return nullptr;
}

void ReplicaPlayerRotationController::UpdateReplicaState() const
{
	auto lastStateData = this->GetLastStateData();

	if (lastStateData.size() > 0)
	{
		hermes::network::NetworkObjectStateDataSPtr stateData = lastStateData.back();
		auto rotationData = std::static_pointer_cast<NetworkObjectRotationData>(stateData);

		hermes::network::ReplicaObject* replicaObject = this->GetReplicaObject();

		if (replicaObject)
		{
			auto replicaPlayer = static_cast<ReplicaPlayer*>(replicaObject);
			gameplay::Vector2D currentLookDirection = gameplay::Vector2D(0, 1).Rotate(replicaPlayer->rotation);
			gameplay::Vector2D lookDirection = gameplay::Vector2D(0, 1).Rotate(rotationData->m_rotation);

			float dot = currentLookDirection.Dot(lookDirection);
			float det = currentLookDirection.Cross(lookDirection);
			float syncAngle = atan2f(det, dot) * 180.f / gameplay::M_PI;

			replicaPlayer->rotation += (syncAngle * 0.1f);
		}
	}
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE