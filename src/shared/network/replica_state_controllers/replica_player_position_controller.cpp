#include "shared/network/replica_state_controllers/replica_player_position_controller.h"
#include "shared/network/network_objects_state_data/network_object_position_data.h"
#include "shared/network/replica_objects/replica_player.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

ReplicaPlayerPositionController::ReplicaPlayerPositionController(hermes::network::NetworkObjectStateId networkObjectStateId, std::uint16_t maxNumOfStateData)
	: ReplicaObjectPositionController(networkObjectStateId, maxNumOfStateData)
{
}

hermes::network::NetworkObjectStateDataSPtr ReplicaPlayerPositionController::GetCurrentStateData() const
{
	hermes::network::ReplicaObject* replicaObject = this->GetReplicaObject();

	if (replicaObject)
	{
		auto replicaPlayer = static_cast<ReplicaPlayer*>(replicaObject);

		auto positionData = std::make_shared<NetworkObjectPositionData>();
		positionData->m_position = replicaPlayer->position;

		return std::static_pointer_cast<hermes::network::NetworkObjectStateData>(positionData);
	}

	return nullptr;
}

void ReplicaPlayerPositionController::UpdateReplicaState() const
{
	auto lastStateData = this->GetLastStateData();

	if (lastStateData.size() > 0)
	{
		hermes::network::NetworkObjectStateDataSPtr stateData = lastStateData.back();
		auto positionData = std::static_pointer_cast<NetworkObjectPositionData>(stateData);

		hermes::network::ReplicaObject* replicaObject = this->GetReplicaObject();

		if (replicaObject)
		{
			auto replicaPlayer = static_cast<ReplicaPlayer*>(replicaObject);
			replicaPlayer->position = gameplay::Lerp(replicaPlayer->position, positionData->m_position, 0.1f);
		}
	}
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE