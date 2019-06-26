#include "shared/network/replica_objects/replica_enemy.h"
#include "shared/network/network_objects_init_data/enemy_init_data.h"
#include "shared/gameplay/engine/rigidbody.h"
#include "shared/gameplay/engine/constants.h"

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

	rigidbody->velocity = gameplay::Vector2D(0) - position;
	rigidbody->velocity.SafeNormalize();
	rigidbody->velocity *= movementSpeed;

	rotation = atan2f(rigidbody->velocity.y, rigidbody->velocity.x) * 180.f / gameplay::M_PI - 90.f;
	life = enemyInitData->m_life >= 0 ? enemyInitData->m_life : life;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE