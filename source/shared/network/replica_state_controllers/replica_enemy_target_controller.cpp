#include "shared/network/replica_state_controllers/replica_enemy_target_controller.h"
#include "shared/network/network_objects_state_data/enemy_target_data.h"
#include "shared/network/replica_objects/replica_enemy.h"
#include "shared/network/synchronization_service.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

ReplicaEnemyTargetController::ReplicaEnemyTargetController(hermes::network::NetworkObjectStateId networkObjectStateId, std::uint16_t maxNumOfStateData) 
	: ReplicaStateController(networkObjectStateId, maxNumOfStateData)
{
}

hermes::network::NetworkObjectStateDataSPtr ReplicaEnemyTargetController::ParseStateData(hermes::network::NetworkObjectStateSPtr objectState) const
{
	if (objectState)
	{
		auto targetData = std::make_shared<EnemyTargetData>();

		BEGIN_NETWORK_MESSAGE_READING(objectState, stateStream)
			targetData->SerializeInternal(stateStream);
		END_NETWORK_MESSAGE_READING(objectState, stateStream)
			
		return std::static_pointer_cast<hermes::network::NetworkObjectStateData>(targetData);
	}

	return nullptr;
}

hermes::network::NetworkObjectStateDataSPtr ReplicaEnemyTargetController::GetCurrentStateData() const
{
	hermes::network::ReplicaObject* replicaObject = this->GetReplicaObject();

	if (replicaObject)
	{
		auto targetData = std::make_shared<EnemyTargetData>();
		auto target = static_cast<ReplicaEnemy*>(replicaObject)->GetTarget();

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

void ReplicaEnemyTargetController::UpdateReplicaState() const
{
	auto lastStateData = this->GetLastStateData();

	if (lastStateData.size() > 0)
	{
		hermes::network::NetworkObjectStateDataSPtr stateData = lastStateData.back();
		auto targetData = std::static_pointer_cast<EnemyTargetData>(stateData);

		hermes::network::ReplicaObject* replicaObject = this->GetReplicaObject();

		if (replicaObject)
		{
			auto replicaEnemy = static_cast<ReplicaEnemy*>(replicaObject);
			
			int idGameObject;
			gameplay::Actor* target = nullptr;

			const auto& networkToLocal = GetSynchronizationService().NetworkToLocal();
			
			if (networkToLocal.GetLinkedId(targetData->m_objectIdValue, idGameObject))
			{
				auto gameObject = gameplay::Object::FindByID(idGameObject);

				if (gameObject->IsTypeOf(gameplay::ObjectType::Actor))
				{
					target = static_cast<gameplay::Actor*> (gameObject);
				}
			}

			replicaEnemy->SetTarget(target);
		}
	}
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE