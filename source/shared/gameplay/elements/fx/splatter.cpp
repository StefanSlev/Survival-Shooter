#include "shared/gameplay/elements/fx/splatter.h"
#include "shared/gameplay/engine/collider.h"
#include "shared/gameplay/engine/resource_manager.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const char* Splatter::explosionAnim = "Data\\Anims\\anim_splatter.png";

Splatter::Splatter() : Explosion()
{
	SpriteAnimationInfo<ExplosionAnimation>* explosionAnimation = new SpriteAnimationInfo<ExplosionAnimation>();
	explosionAnimation->animationType = ExplosionAnimation::ANIMATE;
	explosionAnimation->animationSprite = ResourceManager::GetInstance().GetSprite(ResourceType::ANIMATION, explosionAnim);
	explosionAnimation->frameSize = 64;
	explosionAnimation->startIndex = 0;
	explosionAnimation->animationLength = 16;
	explosionAnimation->fps = 30.f;
	explosionAnimation->looping = false;
	explosionAnimation->OnEnd = &Object::OnEnd;

	spriteAnimation->Load(explosionAnimation);
}

void Splatter::Init()
{
	Explosion::Init();

	this->size = 4.f;
	collider->ComputeDefaultRadius();
	collider->radius /= 1.25f;
	
	spriteAnimation->Play(ExplosionAnimation::ANIMATE);
}

bool Splatter::ResolveCollision(PhysicsObject* pOther)
{
	return pOther->IsTypeOf(ObjectType::Actor);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE