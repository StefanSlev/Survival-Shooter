#include "shared/network/master_objects/master_undead.h"
#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/collider.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

const char* MasterUndead::walkAnim = "Data\\Anims\\anim_undead_walk.png";
const char* MasterUndead::deathAnim = "Data\\Anims\\anim_undead_death.png";
const char* MasterUndead::attackAnim = "Data\\Anims\\anim_undead_attack.png";

MasterUndead::MasterUndead(hermes::network::NetworkObjectId networkObjectId)
	: MasterEnemy(networkObjectId)
{
	Init();
}

void MasterUndead::Init()
{
	MasterEnemy::Init();

	// from Object
	size = 6.f;
	collider->ComputeDefaultRadius();
	collider->radius /= 4.f;

	// from Actor
	this->SetMovementSpeed(2.f);
	this->SetRotationSpeed(50.f);

	// from Enemy
	damage = 5;

	// load sprites and animations
	auto& resourceManager = gameplay::ResourceManager::GetInstance();

	// load animations
	auto walkAnimation = new gameplay::SpriteAnimationInfo<gameplay::ActorState>();
	walkAnimation->animationType = gameplay::ActorState::WALKING;
	walkAnimation->animationSprite = resourceManager.GetSprite(gameplay::ResourceType::ANIMATION, walkAnim);
	walkAnimation->frameSize = 256;
	walkAnimation->startIndex = 0;
	walkAnimation->animationLength = 49;
	walkAnimation->fps = 60.f;
	walkAnimation->looping = true;

	auto deathAnimation = new gameplay::SpriteAnimationInfo<gameplay::ActorState>();
	deathAnimation->animationType = gameplay::ActorState::DYING;
	deathAnimation->animationSprite = resourceManager.GetSprite(gameplay::ResourceType::ANIMATION, deathAnim);
	deathAnimation->frameSize = 256;
	deathAnimation->startIndex = 0;
	deathAnimation->animationLength = 151;
	deathAnimation->fps = 60.f;
	deathAnimation->looping = false;
	deathAnimation->OnEnd = &Object::OnEnd;

	auto attackAnimation = new gameplay::SpriteAnimationInfo<gameplay::ActorState>();
	attackAnimation->animationType = gameplay::ActorState::ATTACKING;
	attackAnimation->animationSprite = resourceManager.GetSprite(gameplay::ResourceType::ANIMATION, attackAnim);
	attackAnimation->frameSize = 256;
	attackAnimation->startIndex = 0;
	attackAnimation->animationLength = 67;
	attackAnimation->fps = 60.f;
	attackAnimation->looping = false;

	spriteAnimation->Load(walkAnimation);
	spriteAnimation->Load(deathAnimation);
	spriteAnimation->Load(attackAnimation);

	// initialize the walking state
	this->UpdateState(gameplay::ActorState::WALKING);
	spriteAnimation->Play(gameplay::ActorState::WALKING);
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE