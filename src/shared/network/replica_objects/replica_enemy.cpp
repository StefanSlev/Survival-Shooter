#include "shared/network/replica_objects/replica_enemy.h"
#include "shared/network/network_objects_init_data/enemy_init_data.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

ReplicaEnemy::ReplicaEnemy(hermes::network::NetworkObjectId networkObjectId) 
	: ReplicaObject(networkObjectId)
{
}

hermes::network::NetworkObjectInitDataSPtr ReplicaEnemy::GetNetworkObjectInit() const
{
	auto enemyInitData = std::make_shared<EnemyInitData>();

	enemyInitData->m_position = position;
	enemyInitData->m_rotation = rotation;
	enemyInitData->m_life = life;

	return std::static_pointer_cast<hermes::network::NetworkObjectInitData>(enemyInitData);
}

void ReplicaEnemy::OnNetworkObjectInit(hermes::network::NetworkObjectInitWrapper& objectInitWrapper)
{
	auto enemyInitData = std::make_shared<EnemyInitData>();

	BEGIN_OBJECT_INIT_DATA_READING(objectInitWrapper, initStream)
		enemyInitData->SerializeInternal(initStream);
	END_OBJECT_INIT_DATA_READING(objectInitWrapper, initStream)

	position = enemyInitData->m_position;
	rotation = enemyInitData->m_rotation;
	life = enemyInitData->m_life;

	UpdateLifeBar();
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE