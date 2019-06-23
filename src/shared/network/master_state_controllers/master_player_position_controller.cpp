#include "shared/network/master_state_controllers/master_player_position_controller.h"
#include "shared/network/network_objects_state_data/network_object_position_data.h"
#include "shared/network/master_objects/master_player.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

MasterPlayerPositionController::MasterPlayerPositionController(hermes::network::NetworkObjectStateId networkObjectStateId, float syncThreshold) 
	: MasterObjectPositionController(networkObjectStateId, syncThreshold)
{
}

hermes::network::NetworkObjectStateDataSPtr MasterPlayerPositionController::GetCurrentStateData() const
{
	hermes::network::MasterObject* masterObject = this->GetMasterObject();

	if (masterObject)
	{
		MasterPlayer* masterPlayer = static_cast<MasterPlayer*> (masterObject);

		auto positionData = std::make_shared<NetworkObjectPositionData>();
		positionData->m_position = masterPlayer->position;

		return std::static_pointer_cast<hermes::network::NetworkObjectStateData>(positionData);
	}

	return nullptr;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE