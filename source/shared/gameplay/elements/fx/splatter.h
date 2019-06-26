#ifndef SHARED_GAMEPLAY_ELEMENTS_SPLATTER
#define SHARED_GAMEPLAY_ELEMENTS_SPLATTER

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/fx/explosion.h"

// -------------------------------------------------------------------------------------
// Class that defines kamikaze's splatter
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Splatter : public Explosion
{
public:
	Splatter();

	ObjectType GetType() const override { return ObjectType::Splatter; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Splatter) || Explosion::IsTypeOf(type); }

	void Init() override;
	bool ResolveCollision(PhysicsObject* pOther) override;

private:
	static const char* explosionAnim;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_SPLATTER