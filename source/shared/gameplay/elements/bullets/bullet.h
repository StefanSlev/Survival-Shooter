#ifndef SHARED_GAMEPLAY_ELEMENTS_BULLET
#define SHARED_GAMEPLAY_ELEMENTS_BULLET

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/weapons/weapon_info.h"
#include "shared/gameplay/engine/physics_object.h"

// -------------------------------------------------------------------------------------
// Class that defines a Bullet.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Bullet : public PhysicsObject
{
public:
	Bullet();
	~Bullet() override;

	ObjectType GetType() const override { return ObjectType::Bullet; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Bullet) || PhysicsObject::IsTypeOf(type); }

	void Update(float dt) override;

	void LoadBulletInfo(const BulletInfo& loadBulletInfo);
	
	void Init() override;
	void Load(Vector2D loadPosition, float loadRotation);

	// returns true if it found a target
	virtual bool ResolveCollision(PhysicsObject* pOther);
	void OnCollisionTrigger(PhysicsObject* pOther) override;
	void OnEnd() override;

	BulletInfo* bulletInfo;
	int idGenerator;

private:
	Vector2D initialPosition;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_BULLET