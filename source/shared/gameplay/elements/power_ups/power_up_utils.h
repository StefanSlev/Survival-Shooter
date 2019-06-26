#ifndef SHARED_GAMEPLAY_ELEMENTS_POWER_UP_UTILS
#define SHARED_GAMEPLAY_ELEMENTS_POWER_UP_UTILS

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/weapons/weapon_info.h"

// -------------------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Actor;

class PowerUpUtils
{
public:
	static void HandleAssault(Actor* actor);
	static void HandleRocket(Actor* actor);
	static void HandleGauss(Actor* actor);
	static void HandleInfiniteAmmo(Actor* actor, bool _switch);
	static void HandleFireRate(Actor* actor, bool _switch);
	static void HandlePushBack(Actor* actor, bool _switch);
	static void HandleInvulnerable(Actor* actor, bool _switch);
	static void HandleFreeze(Actor* actor, bool _switch);
	static void HandleEnemiesFreeze(Actor* actor);
	static void HandleNuke(Actor* actor);
private:
	static void HandleWeapon(Actor* actor, WeaponType weaponType);

	PowerUpUtils();
	PowerUpUtils(const PowerUpUtils&) = delete;
	void operator= (const PowerUpUtils&) = delete;

	static const char* shieldFileName;
	static const char* freezeFileName;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_POWER_UP_UTILS