#include "shared/network/replica_objects/replica_player.h"
#include "shared/network/network_objects_init_data/player_init_data.h"
#include "shared/gameplay/engine/rigidbody.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

ReplicaPlayer::ReplicaPlayer(hermes::network::NetworkObjectId networkObjectId)
	: ReplicaObject(networkObjectId)
{
	Player::Init();
}

hermes::network::NetworkObjectInitDataSPtr ReplicaPlayer::GetNetworkObjectInit() const
{
	auto playerInitData = std::make_shared<PlayerInitData>();

	playerInitData->m_position = position;
	playerInitData->m_rotation = rotation;
	playerInitData->m_life = life;

	return std::static_pointer_cast<hermes::network::NetworkObjectInitData> (playerInitData);
}

void ReplicaPlayer::OnNetworkObjectInit(hermes::network::NetworkObjectInitWrapper& objectInitWrapper)
{
	auto playerInitData = std::make_shared<PlayerInitData>();

	BEGIN_OBJECT_INIT_DATA_READING(objectInitWrapper, initStream)
		playerInitData->SerializeInternal(initStream);
	END_OBJECT_INIT_DATA_READING(objectInitWrapper, initStream)

	position = playerInitData->m_position;
	rotation = playerInitData->m_rotation;
	life = playerInitData->m_life;

	UpdateLifeBar();
}

void ReplicaPlayer::Update(float dt)
{
	// update animation for remote player
	if (spriteAnimation->IsPlaying(gameplay::ActorState::WALKING))
	{
		gameplay::Vector2D lookDirection = gameplay::Vector2D(0, 1).Rotate(rotation);
		float playSpeed = rigidbody->velocity.Dot(lookDirection);

		spriteAnimation->UpdatePlaySpeed(playSpeed);
	}

	Actor::Update(dt);
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE