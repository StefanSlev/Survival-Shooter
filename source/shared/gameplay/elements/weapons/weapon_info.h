#ifndef SHARED_GAMEPLAY_ELEMENTS_WEAPON_INFO
#define SHARED_GAMEPLAY_ELEMENTS_WEAPON_INFO

#include "shared/gameplay/gameplay_base.h"

// -------------------------------------------------------------------------------------
// Weapons and bullets info wrappers
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Sprite;

enum class WeaponType : std::uint32_t
{
	Gun,
	AssaultRifle,
	GaussRifle,
	RocketLauncher
};

enum class BulletType : std::uint32_t
{
	GunBullet,
	AssaultBullet,
	GaussBullet,
	Rocket
};

// use these arrays to iterate over all the elements of the enum
const WeaponType weaponTypes[] = { WeaponType::Gun, WeaponType::AssaultRifle, WeaponType::GaussRifle, WeaponType::RocketLauncher };
const size_t nrWeapons = sizeof(weaponTypes) / sizeof(weaponTypes[0]);

const BulletType bulletTypes[] = { BulletType::GunBullet, BulletType::AssaultBullet, BulletType::GaussBullet, BulletType::Rocket };
const size_t nrBullets = sizeof(bulletTypes) / sizeof(bulletTypes[0]);

#define INVENTORY_SIZE 4
#define INFINITE_AMMO -1
#define INFINITE_CLIPS -1

struct BulletInfo
{
	WeaponType weaponType;
	BulletType bulletType;

	Sprite* bulletSprite;
	float barrelOffset;
	float bulletSpeed;
	int damage;
	float range;
	
	float pushBackPower;
	bool pushBack;

	// in-game properties
	float size;
	long color;

	BulletInfo()
	{
		bulletSprite = nullptr;
		barrelOffset = 1.f;
		bulletSpeed = 0.f;
		damage = 0;
		range = 0.f;
		size = 1.f;

		pushBackPower = 0.f;
		pushBack = false;
	}
};

struct WeaponInfo
{
	WeaponInfo() {

		weaponSprite = nullptr;
		rateOfFire = 0.f;
		reloadTime = 0.f;
		numberOfClips = 0;
		clipSize = 0;
		bulletsLoaded = 0;
	}

	WeaponType weaponType;
	BulletType bulletType;

	Sprite* weaponSprite;
	float rateOfFire;
	float reloadTime;
	int numberOfClips; // use INFINITE_CLIPS for infinite value
	int clipSize;
	int bulletsLoaded; // use INFINITE_AMMO for infinite value
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_WEAPON_INFO