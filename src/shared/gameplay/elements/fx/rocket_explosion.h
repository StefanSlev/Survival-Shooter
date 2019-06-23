#ifndef SHARED_GAMEPLAY_ELEMENTS_ROCKET_EXPLOSION
#define SHARED_GAMEPLAY_ELEMENTS_ROCKET_EXPLOSION

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/fx/explosion.h"

// -------------------------------------------------------------------------------------
// Class that defines a rocket explosion
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class RocketExplosion : public Explosion
{
public:
	RocketExplosion();

	ObjectType GetType() const override { return ObjectType::RocketExplosion; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::RocketExplosion) || Explosion::IsTypeOf(type); }

	void Init() override;
	bool ResolveCollision(PhysicsObject* pOther) override;

private:
	static const char* explosionAnim;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_ROCKET_EXPLOSION