#include "shared/gameplay/elements/fx/rocket_explosion.h"
#include "shared/gameplay/engine/collider.h"
#include "shared/gameplay/engine/resource_manager.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const char* RocketExplosion::explosionAnim = "Data\\Anims\\anim_explosion1.png";

RocketExplosion::RocketExplosion() : Explosion()
{
	SpriteAnimationInfo<ExplosionAnimation>* explosionAnimation = new SpriteAnimationInfo<ExplosionAnimation>();
	explosionAnimation->animationType = ExplosionAnimation::ANIMATE;
	explosionAnimation->animationSprite = ResourceManager::GetInstance().GetSprite(ResourceType::ANIMATION, explosionAnim);
	explosionAnimation->frameSize = 64;
	explosionAnimation->startIndex = 0;
	explosionAnimation->animationLength = 42;
	explosionAnimation->fps = 60.f;
	explosionAnimation->looping = false;
	explosionAnimation->OnEnd = &Object::OnEnd;

	spriteAnimation->Load(explosionAnimation);
}

void RocketExplosion::Init()
{
	Explosion::Init();

	this->size = 6.f;
	collider->ComputeDefaultRadius();
	collider->radius /= 1.25f;

	spriteAnimation->Play(ExplosionAnimation::ANIMATE);
}

bool RocketExplosion::ResolveCollision(PhysicsObject* pOther)
{
	return pOther->IsTypeOf(ObjectType::Enemy);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE