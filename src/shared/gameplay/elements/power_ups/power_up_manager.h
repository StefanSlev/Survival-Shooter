#ifndef SHARED_GAMEPLAY_ELEMENTS_POWER_UP_MANAGER
#define SHARED_GAMEPLAY_ELEMENTS_POWER_UP_MANAGER

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/power_ups/power_up_info.h"

// -------------------------------------------------------------------------------------
// Class that defines the powerUps manager
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class PowerUpManager
{
public:
	static PowerUpManager& GetInstance()
	{
		static PowerUpManager instance;

		return instance;
	}
	PowerUpEffect* GetPowerUpEffect(PowerUpType powerUpType);
	~PowerUpManager();

private:
	std::unordered_map<PowerUpType, PowerUpEffect*> powerUpEffects;
	
	PowerUpManager();
	PowerUpManager(const PowerUpManager&) = delete;
	void operator= (const PowerUpManager&) = delete;

	static const char* assault_path;
	static const char* gauss_path;
	static const char* rocket_path;
	static const char* infinite_ammo_path;
	static const char* fire_rate_path;
	static const char* push_back_path;
	static const char* invulnerable_path;
	static const char* freeze_path;
	static const char* nuke_path;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_POWER_UP_MANAGER