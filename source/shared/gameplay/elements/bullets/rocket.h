#ifndef SHARED_GAMEPLAY_ELEMENTS_ROCKET
#define SHARED_GAMEPLAY_ELEMENTS_ROCKET

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/bullets/bullet.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

// -------------------------------------------------------------------------------------
// Class that defines a Rocket
// -------------------------------------------------------------------------------------

class Rocket final : public Bullet
{
public:
	Rocket();

	ObjectType GetType() const override { return ObjectType::Rocket; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Rocket) || Bullet::IsTypeOf(type); }

	bool ResolveCollision(PhysicsObject* pOther) override;
	void OnEnd() override;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_ROCKET