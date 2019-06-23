#include "shared/network/replica_state_controllers/replica_object_position_controller.h"
#include "shared/network/network_objects_state_data/network_object_position_data.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

ReplicaObjectPositionController::ReplicaObjectPositionController(hermes::network::NetworkObjectStateId networkObjectStateId, std::uint16_t maxNumOfStateData) 
	: ReplicaStateController(networkObjectStateId, maxNumOfStateData)
{
}

hermes::network::NetworkObjectStateDataSPtr ReplicaObjectPositionController::ParseStateData(hermes::network::NetworkObjectStateSPtr objectState) const
{
	if (objectState)
	{
		auto positionData = std::make_shared<NetworkObjectPositionData>();

		BEGIN_NETWORK_MESSAGE_READING(objectState, stateStream)
			positionData->SerializeInternal(stateStream);
		END_NETWORK_MESSAGE_READING(objectState, stateStream)

		return std::static_pointer_cast<hermes::network::NetworkObjectStateData>(positionData);
	}

	return nullptr;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE