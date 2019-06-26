#include "shared/network/master_state_controllers/master_player_rotation_controller.h"
#include "shared/network/network_objects_state_data/network_object_rotation_data.h"
#include "shared/network/master_objects/master_player.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

MasterPlayerRotationController::MasterPlayerRotationController(hermes::network::NetworkObjectStateId networkObjectStateId, float syncThreshold)
	: MasterObjectRotationController(networkObjectStateId, syncThreshold)
{
}

hermes::network::NetworkObjectStateDataSPtr MasterPlayerRotationController::GetCurrentStateData() const
{
	hermes::network::MasterObject* masterObject = this->GetMasterObject();

	if (masterObject)
	{
		auto masterPlayer = static_cast<MasterPlayer*> (masterObject);

		auto rotationData = std::make_shared<NetworkObjectRotationData>();
		rotationData->m_rotation = masterPlayer->rotation;

		return std::static_pointer_cast<hermes::network::NetworkObjectStateData>(rotationData);
	}

	return nullptr;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE