#ifndef SHARED_GAMEPLAY_ELEMENTS_WEAPON_INFO_MANAGER
#define SHARED_GAMEPLAY_ELEMENTS_WEAPON_INFO_MANAGER

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/weapons/weapon_info.h"

// -------------------------------------------------------------------------------------
// Class that defines the weapons manager
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

struct SpritesPath
{
	const char* weaponSprite;
	const char* bulletSprite;
};

class WeaponInfoManager
{
public:
	static WeaponInfoManager& GetInstance()
	{
		static WeaponInfoManager instance;

		return instance;
	}
	WeaponInfo* GetWeaponInfo(WeaponType weaponType);
	BulletInfo* GetBulletInfo(BulletType bulletType);

	~WeaponInfoManager();
private:

	std::unordered_map<WeaponType, WeaponInfo*> weaponsInfo;
	std::unordered_map<BulletType, BulletInfo*> bulletsInfo;

	WeaponInfoManager();
	WeaponInfoManager(const WeaponInfoManager&) = delete;
	void operator= (const WeaponInfoManager&) = delete;

	static const SpritesPath gun;
	static const SpritesPath assault_rifle;
	static const SpritesPath gauss_rifle;
	static const SpritesPath rocket_launcher;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_WEAPON_INFO_MANAGER