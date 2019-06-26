#include "shared/network/master_state_controllers/master_enemy_target_controller.h"
#include "shared/network/network_objects_state_data/enemy_target_data.h"
#include "shared/network/master_objects/master_enemy.h"
#include "shared/network/synchronization_service.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

MasterEnemyTargetController::MasterEnemyTargetController(hermes::network::NetworkObjectStateId networkObjectStateId, float syncThreshold) 
	: MasterStateController(networkObjectStateId, syncThreshold)
{
}

hermes::network::NetworkObjectStateDataSPtr MasterEnemyTargetController::GetCurrentStateData() const
{
	hermes::network::MasterObject* masterObject = this->GetMasterObject();

	if (masterObject)
	{
		auto targetData = std::make_shared<EnemyTargetData>();
		auto target = static_cast<MasterEnemy*> (masterObject)->GetTarget();

		if (target)
		{
			hermes::network::NetworkObjectIdValue objectIdValue;
			const auto& localToNetwork = GetSynchronizationService().LocalToNetwork();

			if (localToNetwork.GetLinkedId(target->GetID(), objectIdValue))
			{
				targetData->m_objectIdValue = objectIdValue;
			}
		}

		return std::static_pointer_cast<hermes::network::NetworkObjectStateData>(targetData);
	}

	return nullptr;
}

float MasterEnemyTargetController::ComputeSyncRate(hermes::network::NetworkObjectStateDataSPtr stateData) const
{
	bool changedTarget = false;
	bool differentTargets = false;

	hermes::network::MasterObject* masterObject = this->GetMasterObject();

	if (masterObject)
	{
		auto masterEnemy = static_cast<MasterEnemy*> (masterObject);
		changedTarget = masterEnemy->ChangedTarget();
	}

	if (stateData)
	{
		auto targetData = std::static_pointer_cast<EnemyTargetData>(stateData);
		hermes::network::NetworkObjectStateDataSPtr currentStateData = GetCurrentStateData();

		if (currentStateData)
		{
			auto currentTargetData = std::static_pointer_cast<EnemyTargetData>(currentStateData);
			differentTargets = (currentTargetData->m_objectIdValue != targetData->m_objectIdValue);
		}
	}

	return (changedTarget || differentTargets) ? 0.f : 1.f;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE