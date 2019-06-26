#include "shared/network/replica_state_controllers/replica_object_rotation_controller.h"
#include "shared/network/network_objects_state_data/network_object_rotation_data.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

ReplicaObjectRotationController::ReplicaObjectRotationController(hermes::network::NetworkObjectStateId networkObjectStateId, std::uint16_t maxNumOfStateData)
	: ReplicaStateController(networkObjectStateId, maxNumOfStateData)
{
}

hermes::network::NetworkObjectStateDataSPtr ReplicaObjectRotationController::ParseStateData(hermes::network::NetworkObjectStateSPtr objectState) const
{
	if (objectState)
	{
		auto rotationData = std::make_shared<NetworkObjectRotationData>();

		BEGIN_NETWORK_MESSAGE_READING(objectState, stateStream)
			rotationData->SerializeInternal(stateStream);
		END_NETWORK_MESSAGE_READING(objectState, stateStream)

		return std::static_pointer_cast<hermes::network::NetworkObjectStateData>(rotationData);
	}

	return nullptr;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE