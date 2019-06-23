#ifndef SHARED_GAMEPLAY_ELEMENTS_WEAPON
#define SHARED_GAMEPLAY_ELEMENTS_WEAPON

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/controller.h"
#include "shared/gameplay/elements/weapons/weapon_info.h"

// -------------------------------------------------------------------------------------
// Class that defines the weapon.
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

/**
*	WeaponInfoGlobal represents weapon and bullets properties that are modifiable
* during the game by taking power-ups
**/

struct WeaponInfoGlobal
{
	WeaponInfoGlobal() {}

	void Init()
	{
		rateOfFireProc = 1.f; // should be multiplied with weapon's rateOfFire
		infinite_ammo = false;
		infinite_clips = false;
		bulletSpeedProc = 1.f; // should be multiplied with weapon's bulletSpeed
		extraDamage = 0; // should be added to weapon's damage
		extraRange = 0.f; // should be added to weapon's range
		pushBack = false; // should be verified together with the weapon's bullet pushBack property
	}

	float rateOfFireProc;
	bool infinite_ammo;
	bool infinite_clips;
	float bulletSpeedProc;
	int extraDamage;
	float extraRange;
	bool pushBack;
};

class Crosshair;

class Weapon final : public Controller
{
public:
	Weapon();
	void Init();

	~Weapon() override;
	
	ObjectType GetType() const override { return ObjectType::Weapon; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Weapon) || Controller::IsTypeOf(type); }

	void Update(float dt) override;

	void ChangeWeapon(WeaponType weaponType);
	void ApplyWeaponPowerUp(WeaponType weaponType);

	void Refresh();
	void FireBullet();

private:
	WeaponInfo* currentWeapon;
	WeaponInfoGlobal globalInfo;

	Crosshair* crosshair;
	float waitTime;
	float reloadTime;

	// properties for all weapons held
	std::unordered_map<WeaponType, WeaponInfo*> weapons;

	// method uses both globalInfo and weaponInfo
	BulletInfo ComputeFinalBulletInfo(const BulletInfo& bulletInfo) const;
	void LoadFirstClip(WeaponInfo* weapon);

	static const char* crosshairFileName;
	friend class PlayerHUD;
	friend class PowerUpUtils;
	friend class Player;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_WEAPON