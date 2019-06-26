#include "shared/network/replica_objects/replica_kamikaze.h"

#include "shared/gameplay/elements/fx/splatter.h"
#include "shared/gameplay/elements/game.h"

#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/object_pools.h"
#include "shared/gameplay/engine/collider.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

const char* ReplicaKamikaze::walkAnim = "Data\\Anims\\anim_kamikaze_walk.png";
const char* ReplicaKamikaze::deathAnim = "Data\\Anims\\anim_kamikaze_death.png";

ReplicaKamikaze::ReplicaKamikaze(hermes::network::NetworkObjectId networkObjectId)
	: ReplicaEnemy(networkObjectId)
{
	Init();
}

void ReplicaKamikaze::Init()
{
	ReplicaEnemy::Init();

	// from Object
	size = 4.f;
	collider->ComputeDefaultRadius();
	collider->radius /= 3.f;

	// from Actor
	this->SetMovementSpeed(3.f);
	this->SetRotationSpeed(180.f);

	// from Enemy
	damage = 10;
	hitRate = 0.f;
	canAttack = false;

	// load sprites and animations
	auto& resourceManager = gameplay::ResourceManager::GetInstance();

	// load animations
	auto walkAnimation = new gameplay::SpriteAnimationInfo<gameplay::ActorState>();
	walkAnimation->animationType = gameplay::ActorState::WALKING;
	walkAnimation->animationSprite = resourceManager.GetSprite(gameplay::ResourceType::ANIMATION, walkAnim);
	walkAnimation->frameSize = 256;
	walkAnimation->startIndex = 0;
	walkAnimation->animationLength = 51;
	walkAnimation->fps = 60.f;
	walkAnimation->looping = true;

	auto deathAnimation = new gameplay::SpriteAnimationInfo<gameplay::ActorState>();
	deathAnimation->animationType = gameplay::ActorState::DYING;
	deathAnimation->animationSprite = resourceManager.GetSprite(gameplay::ResourceType::ANIMATION, deathAnim);
	deathAnimation->frameSize = 256;
	deathAnimation->startIndex = 0;
	deathAnimation->animationLength = 101;
	deathAnimation->fps = 60.f;
	deathAnimation->looping = false;
	deathAnimation->OnEnd = &Object::OnEnd;

	spriteAnimation->Load(walkAnimation);
	spriteAnimation->Load(deathAnimation);

	// initialize the walking state
	this->UpdateState(gameplay::ActorState::WALKING);
	spriteAnimation->Play(gameplay::ActorState::WALKING);
}

void ReplicaKamikaze::OnEnd()
{
	auto currentState = this->GetCurrentState();

	if (currentState != gameplay::ActorState::DYING && currentState != gameplay::ActorState::ENDING)
	{
		Object* splatter = gameplay::ObjectPools::GetInstance().GetObjectFromPool(ObjectType::Splatter);

		static_cast<gameplay::Explosion*> (splatter)->Load(GetID(), damage);
		splatter->position = position;

		gameplay::Game::g_pGame->AddGameObject(splatter);
	}

	ReplicaEnemy::OnEnd();
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE