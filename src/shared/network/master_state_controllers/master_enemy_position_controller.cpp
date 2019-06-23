#include "shared/network/master_state_controllers/master_enemy_position_controller.h"
#include "shared/network/network_objects_state_data/network_object_position_data.h"
#include "shared/network/master_objects/master_enemy.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

MasterEnemyPositionController::MasterEnemyPositionController(hermes::network::NetworkObjectStateId networkObjectStateId, float syncThreshold)
	: MasterObjectPositionController(networkObjectStateId, syncThreshold)
{
}

hermes::network::NetworkObjectStateDataSPtr MasterEnemyPositionController::GetCurrentStateData() const
{
	hermes::network::MasterObject* masterObject = this->GetMasterObject();

	if (masterObject)
	{
		auto masterEnemy = static_cast<MasterEnemy*> (masterObject);

		auto positionData = std::make_shared<NetworkObjectPositionData>();
		positionData->m_position = masterEnemy->position;

		return std::static_pointer_cast<hermes::network::NetworkObjectStateData>(positionData);
	}

	return nullptr;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE