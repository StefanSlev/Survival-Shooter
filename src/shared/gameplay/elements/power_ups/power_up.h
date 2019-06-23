#ifndef SHARED_GAMEPLAY_ELEMENTS_POWER_UP
#define SHARED_GAMEPLAY_ELEMENTS_POWER_UP

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/power_ups/power_up_info.h"
#include "shared/gameplay/engine/physics_object.h"
#include "shared/gameplay/engine/utils.h"

// -------------------------------------------------------------------------------------
// Class that defines a PowerUp
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class PowerUp final : public PhysicsObject
{
public:
	PowerUp();

	ObjectType GetType() const override { return ObjectType::PowerUp; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::PowerUp) || PhysicsObject::IsTypeOf(type); }
	
	void Init() override;
	void Load(PowerUpType loadPowerUpType);

	void Update(float dt) override;
	void OnEnd() override;

	void OnCollisionTrigger(PhysicsObject* pOther) override;

	PowerUpType powerUpType;

private:
	float lifeTime;
	int triggerID;

	const float totalLifeTime = 15.f;
	const float totalBlinkTime = 3.f;
	const int nrBlinks = 10;
	const long transparentMask = sColor(100, 255, 255, 255).GetValue();
	const long visibleMask = sColor(255, 255, 255, 255).GetValue();
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_POWER_UP