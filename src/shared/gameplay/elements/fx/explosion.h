#ifndef SHARED_GAMEPLAY_ELEMENTS_EXPLOSION
#define SHARED_GAMEPLAY_ELEMENTS_EXPLOSION

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/physics_object.h"
#include "shared/gameplay/engine/sprite_animation_controller.h"

// -------------------------------------------------------------------------------------
// Class that defines an explosion.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Explosion : public PhysicsObject
{
public:
	enum class ExplosionAnimation {

		ANIMATE
	};

	Explosion();

	void Init() override;
	void Load(int loadIdGenerator, int loadDamage);

	ObjectType GetType() const override { return ObjectType::Explosion; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Explosion) || PhysicsObject::IsTypeOf(type); }

	virtual bool ResolveCollision(PhysicsObject* pOther) = 0;
	void OnCollisionTrigger(PhysicsObject* pOther) override;

protected:
	int damage;
	int idGenerator;
	SpriteAnimationController<ExplosionAnimation>* spriteAnimation;

private:
	std::vector<int> hitIDs;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_EXPLOSION