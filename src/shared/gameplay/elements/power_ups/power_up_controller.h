#ifndef SHARED_GAMEPLAY_ELEMENTS_POWER_UP_CONTROLLER
#define SHARED_GAMEPLAY_ELEMENTS_POWER_UP_CONTROLLER

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/power_ups/power_up_info.h"
#include "shared/gameplay/engine/controller.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class CountdownClock;
class Actor;

class PowerUpController final : public Controller
{
public:
	PowerUpController();

	ObjectType GetType() const override { return ObjectType::PowerUpController; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::PowerUpController) || Controller::IsTypeOf(type); }

	void Update(float dt) override;

	void ApplyPowerUp(PowerUpType powerUpType);
	void RemovePowerUp(PowerUpType powerUpType);

	void ClearPowerUps();

private:
	void RefreshClocks();
	void SetObject(Object * _object) override;
	
	std::unordered_map<PowerUpType, CountdownClock*> powerUps;

	// the actor that is controlled
	Actor* actor;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_POWER_UP_CONTROLLER