#include "shared/gameplay/elements/fx/bullet_explosion.h"
#include "shared/gameplay/engine/resource_manager.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const char* BulletExplosion::explosionAnim = "Data\\Anims\\anim_explosion2.png";

BulletExplosion::BulletExplosion()
{
	spriteAnimation = new SpriteAnimationController<ExplosionAnimation>();
	this->AddController(spriteAnimation);

	SpriteAnimationInfo<ExplosionAnimation>* explosionAnimation = new SpriteAnimationInfo<ExplosionAnimation>();
	explosionAnimation->animationType = ExplosionAnimation::ANIMATE;
	explosionAnimation->animationSprite = ResourceManager::GetInstance().GetSprite(ResourceType::ANIMATION, explosionAnim);
	explosionAnimation->frameSize = 64;
	explosionAnimation->startIndex = 0;
	explosionAnimation->animationLength = 16;
	explosionAnimation->fps = 60.f;
	explosionAnimation->looping = false;
	explosionAnimation->OnEnd = &Object::OnEnd;

	spriteAnimation->Load(explosionAnimation);
}

void BulletExplosion::Init()
{
	WorldObject::Init();

	this->size = 0.75f;

	spriteAnimation->Play(ExplosionAnimation::ANIMATE);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE