#include "shared/gameplay/elements/weapons/weapon_info_manager.h"
#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/utils.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const SpritesPath WeaponInfoManager::gun = { "Data\\Images\\gun.png",  "Data\\Images\\gun_bullet.png" };
const SpritesPath WeaponInfoManager::assault_rifle = { "Data\\Images\\assault_rifle.png",  "Data\\Images\\assault_bullet.png" };
const SpritesPath WeaponInfoManager::gauss_rifle = { "Data\\Images\\gauss_rifle.png",  "Data\\Images\\gauss_bullet.png" };
const SpritesPath WeaponInfoManager::rocket_launcher = { "Data\\Images\\rocket_launcher.png",  "Data\\Images\\rocket_bullet.png" };

WeaponInfoManager::~WeaponInfoManager()
{
	auto delete_info = [](auto elem) { delete elem.second; };

	std::for_each(weaponsInfo.begin(), weaponsInfo.end(), delete_info);
	std::for_each(bulletsInfo.begin(), bulletsInfo.end(), delete_info);
}

template <class Info, class Type> Info* GetInfo(const std::unordered_map<Type, Info*>& infos, Type type)
{
	auto it = infos.find(type);

	if (it != infos.end())
		return it->second;

	return nullptr;
}

WeaponInfo* WeaponInfoManager::GetWeaponInfo(WeaponType weaponType)
{
	// get the info asociated with the given weapon
	return GetInfo(weaponsInfo, weaponType);
}

BulletInfo* WeaponInfoManager::GetBulletInfo(BulletType bulletType)
{
	// get the info asociated with the given bullet
	return GetInfo(bulletsInfo, bulletType);
}

WeaponInfoManager::WeaponInfoManager()
{
	// load infos for weapons
	ResourceManager& resourceManager = ResourceManager::GetInstance();

	// gun
	WeaponInfo* gunInfo = new WeaponInfo();
	BulletInfo* gunBulletInfo = new BulletInfo();

	gunInfo->weaponType = WeaponType::Gun;
	gunInfo->bulletType = BulletType::GunBullet;
	gunInfo->weaponSprite = resourceManager.GetSprite(ResourceType::IMAGE, gun.weaponSprite);
	gunInfo->rateOfFire = 3.f;
	gunInfo->reloadTime = 0.7f;
	gunInfo->numberOfClips = INFINITE_CLIPS;
	gunInfo->clipSize = 12;

	gunBulletInfo->bulletType = BulletType::GunBullet;
	gunBulletInfo->weaponType = WeaponType::Gun;
	gunBulletInfo->bulletSprite = resourceManager.GetSprite(ResourceType::IMAGE, gun.bulletSprite);
	gunBulletInfo->damage = 5;
	gunBulletInfo->range = 15.f;
	gunBulletInfo->bulletSpeed = 30.f;
	gunBulletInfo->pushBackPower = 10.f;
	gunBulletInfo->pushBack = true;
	
	gunBulletInfo->size = 0.7f;
	gunBulletInfo->color = sColor(255, 255, 255, 102).GetValue();

	// assault_rifle
	WeaponInfo* assaultInfo = new WeaponInfo();
	BulletInfo* assaultBulletInfo = new BulletInfo();

	assaultInfo->weaponType = WeaponType::AssaultRifle;
	assaultInfo->bulletType = BulletType::AssaultBullet;
	assaultInfo->weaponSprite = resourceManager.GetSprite(ResourceType::IMAGE, assault_rifle.weaponSprite);
	assaultInfo->rateOfFire = 10.f;
	assaultInfo->reloadTime = 1.5f;
	assaultInfo->numberOfClips = 5;
	assaultInfo->clipSize = 50;

	assaultBulletInfo->bulletType = BulletType::AssaultBullet;
	assaultBulletInfo->weaponType = WeaponType::AssaultRifle;
	assaultBulletInfo->bulletSprite = resourceManager.GetSprite(ResourceType::IMAGE, assault_rifle.bulletSprite);
	assaultBulletInfo->damage = 5;
	assaultBulletInfo->range = 15.f;
	assaultBulletInfo->bulletSpeed = 50.f;
	assaultBulletInfo->pushBackPower = 4.f;
	assaultBulletInfo->pushBack = true;

	assaultBulletInfo->size = 0.7f;
	assaultBulletInfo->color = sColor(255, 255, 153, 51).GetValue();

	// gauss_rifle
	WeaponInfo* gaussInfo = new WeaponInfo();
	BulletInfo* gaussBulletInfo = new BulletInfo();

	gaussInfo->weaponType = WeaponType::GaussRifle;
	gaussInfo->bulletType = BulletType::GaussBullet;
	gaussInfo->weaponSprite = resourceManager.GetSprite(ResourceType::IMAGE, gauss_rifle.weaponSprite);
	gaussInfo->rateOfFire = 2.f;
	gaussInfo->reloadTime = 1.5f;
	gaussInfo->numberOfClips = 6;
	gaussInfo->clipSize = 6;
	
	gaussBulletInfo->bulletType = BulletType::GaussBullet;
	gaussBulletInfo->weaponType = WeaponType::GaussRifle;
	gaussBulletInfo->bulletSprite = resourceManager.GetSprite(ResourceType::IMAGE, gauss_rifle.bulletSprite);
	gaussBulletInfo->damage = 15;
	gaussBulletInfo->range = 20.f;
	gaussBulletInfo->bulletSpeed = 50.f;

	gaussBulletInfo->size = 1.2f;
	gaussBulletInfo->color = sColor(255, 0, 204, 102).GetValue();

	// rocket_launcher
	WeaponInfo* rocketLauncherInfo = new WeaponInfo();
	BulletInfo* rocketInfo = new BulletInfo();

	rocketLauncherInfo->weaponType = WeaponType::RocketLauncher;
	rocketLauncherInfo->bulletType = BulletType::Rocket;
	rocketLauncherInfo->weaponSprite = resourceManager.GetSprite(ResourceType::IMAGE, rocket_launcher.weaponSprite);
	rocketLauncherInfo->rateOfFire = 1.f;
	rocketLauncherInfo->reloadTime = 1.f;
	rocketLauncherInfo->numberOfClips = 10;
	rocketLauncherInfo->clipSize = 1;

	rocketInfo->bulletType = BulletType::Rocket;
	rocketInfo->bulletSprite = resourceManager.GetSprite(ResourceType::IMAGE, rocket_launcher.bulletSprite);
	rocketInfo->damage = 30;
	rocketInfo->range = 15.f;
	rocketInfo->bulletSpeed = 15.f;

	rocketInfo->size = 0.7f;
	rocketInfo->color = sColor(255, 153, 0, 51).GetValue();

	weaponsInfo[WeaponType::Gun] = gunInfo;
	bulletsInfo[BulletType::GunBullet] = gunBulletInfo;

	weaponsInfo[WeaponType::AssaultRifle] = assaultInfo;
	bulletsInfo[BulletType::AssaultBullet] = assaultBulletInfo;

	weaponsInfo[WeaponType::GaussRifle] = gaussInfo;
	bulletsInfo[BulletType::GaussBullet] = gaussBulletInfo;

	weaponsInfo[WeaponType::RocketLauncher] = rocketLauncherInfo;
	bulletsInfo[BulletType::Rocket] = rocketInfo;
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE