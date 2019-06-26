#ifndef SHARED_GAMEPLAY_ELEMENTS_BULLET_EXPLOSION
#define SHARED_GAMEPLAY_ELEMENTS_BULLET_EXPLOSION

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/object.h"
#include "shared/gameplay/engine/sprite_animation_controller.h"

// -------------------------------------------------------------------------------------
// Class that defines an explosion.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class BulletExplosion : public WorldObject
{
public:
	enum class ExplosionAnimation {

		ANIMATE
	};

	BulletExplosion();

	void Init() override;

	ObjectType GetType() const override { return ObjectType::BulletExplosion; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::BulletExplosion) || WorldObject::IsTypeOf(type); }

private:
	SpriteAnimationController<ExplosionAnimation>* spriteAnimation;
	static const char* explosionAnim;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_BULLET_EXPLOSION