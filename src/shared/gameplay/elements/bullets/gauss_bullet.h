#ifndef SHARED_GAMEPLAY_ELEMENTS_GAUSS_BULLET
#define SHARED_GAMEPLAY_ELEMENTS_GAUSS_BULLET

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/bullets/bullet.h"

// -------------------------------------------------------------------------------------
// Class that defines a Gauss Bullet.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class GaussBullet final : public Bullet
{
public:
	GaussBullet();

	void Init() override;
	void OnEnd() override;

	ObjectType GetType() const override { return ObjectType::GaussBullet; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::GaussBullet) || Bullet::IsTypeOf(type); }

	void OnCollisionTrigger(PhysicsObject* pOther) override;

private:
	std::vector<int> hitIDs;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_GAUSS_BULLET