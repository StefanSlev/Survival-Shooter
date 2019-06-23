#include "shared/gameplay/elements/weapons/weapon.h"
#include "shared/gameplay/elements/weapons/weapon_info_manager.h"
#include "shared/gameplay/elements/utilities/crosshair.h"
#include "shared/gameplay/elements/game_events/game_event.h"
#include "shared/gameplay/elements/game_events/game_events_queue.h"
#include "shared/gameplay/elements/game.h"

#include "shared/gameplay/engine/resource_manager.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const char* Weapon::crosshairFileName = "Data\\Images\\reticule.png";

Weapon::Weapon() : Controller()
{
	crosshair = nullptr;
}

void Weapon::Init()
{
	/** initialize global info **/
	globalInfo.Init();

	/** add Crosshair **/
	if (crosshair == nullptr)
	{
		crosshair = new Crosshair(ResourceManager::GetInstance().GetSprite(ResourceType::IMAGE, crosshairFileName));
		object->AddChild(crosshair);
	}

	WeaponInfoManager& manager = WeaponInfoManager::GetInstance();

	for (const auto& weaponType : weaponTypes)
	{
		WeaponInfo* defWeaponInfo = manager.GetWeaponInfo(weaponType);

		if (!defWeaponInfo)
			continue;

		auto it = weapons.find(weaponType);

		WeaponInfo* currWeaponInfo = nullptr;

		if (it != weapons.end())
		{
			currWeaponInfo = it->second;
			currWeaponInfo->bulletsLoaded = defWeaponInfo->bulletsLoaded;
			currWeaponInfo->numberOfClips = defWeaponInfo->numberOfClips;
		}
		else
		{
			currWeaponInfo = new WeaponInfo(*manager.GetWeaponInfo(weaponType));
			weapons[weaponType] = currWeaponInfo;
		}

		// drop all the clips at the start of the game
		if (currWeaponInfo->numberOfClips != INFINITE_CLIPS)
			currWeaponInfo->numberOfClips = 0;

		this->LoadFirstClip(currWeaponInfo);
	}

	waitTime = 0.f;
	reloadTime = 0.f;
	currentWeapon = weapons[WeaponType::Gun];
}

void Weapon::Refresh()
{
	if (crosshair)
	{
		WeaponInfoManager& weaponInfoManager = WeaponInfoManager::GetInstance();

		if (currentWeapon != nullptr)
		{
			BulletInfo* bulletInfo = weaponInfoManager.GetBulletInfo(currentWeapon->bulletType);

			float crosshairRadius = fmaxf(crosshair->size.x, crosshair->size.y) / 2;

			crosshair->position = Vector2D(0, bulletInfo->range + bulletInfo->barrelOffset + globalInfo.extraRange + crosshairRadius);
		}

		crosshair->color = object->color;
	}
}

Weapon::~Weapon()
{
	std::for_each(weapons.begin(), weapons.end(), [](auto elem) {
		delete elem.second;
	});
}

void Weapon::ChangeWeapon(WeaponType weaponType)
{
	if (reloadTime > 0.f || waitTime > 0.f)
		return;

	if (weapons.find(weaponType) != weapons.end())
	{
		WeaponInfo* weaponInfo = weapons[weaponType];

		bool infiniteClips = weaponInfo->numberOfClips == INFINITE_CLIPS || globalInfo.infinite_clips;
		bool infiniteAmmo = weaponInfo->bulletsLoaded == INFINITE_AMMO || globalInfo.infinite_ammo;

		if (infiniteClips || infiniteAmmo || weaponInfo->numberOfClips > 0
			|| weaponInfo->bulletsLoaded > 0)
			currentWeapon = weaponInfo;
		else
			currentWeapon = weapons[WeaponType::Gun];

		this->Refresh();
	}
}

void Weapon::FireBullet()
{
	if (!currentWeapon)
		return;

	// if press to shoot
	if (waitTime == 0.f && reloadTime == 0.f)
	{
		bool infiniteAmmo = currentWeapon->bulletsLoaded == INFINITE_AMMO || globalInfo.infinite_ammo;

		if (currentWeapon->bulletsLoaded > 0 || infiniteAmmo)
		{
			GameEvent* gameEvent = new BulletShotEvent(object->GetID(), object->position, object->rotation, currentWeapon->bulletType);
			Game::g_pGame->gameEventsQueue->PushEvent(gameEvent);

			// weapon management
			if (!infiniteAmmo)
				currentWeapon->bulletsLoaded--;

			waitTime = 1.f / (currentWeapon->rateOfFire * globalInfo.rateOfFireProc);
		}
	}
}

void Weapon::Update(float dt)
{
	if (!currentWeapon)
		return;

	if (currentWeapon->bulletsLoaded == 0)
	{
		// if there are infinite clips for a weapon or there are still some unused
		bool infiniteClips = currentWeapon->numberOfClips == INFINITE_CLIPS || globalInfo.infinite_clips;
		bool infiniteAmmo = currentWeapon->bulletsLoaded == INFINITE_AMMO || globalInfo.infinite_ammo;

		if (!infiniteAmmo)
		{
			waitTime = 0.f;

			if (currentWeapon->numberOfClips > 0 || infiniteClips)
			{
				reloadTime = currentWeapon->reloadTime;
				currentWeapon->bulletsLoaded = currentWeapon->clipSize;

				if (!infiniteClips)
					currentWeapon->numberOfClips--;
			}
			else
				this->ChangeWeapon(WeaponType::Gun);
		}
	}

	waitTime = fmaxf(waitTime - dt, 0.f);
	reloadTime = fmaxf(reloadTime - dt, 0.f);
	Controller::Update(dt);
}

void Weapon::ApplyWeaponPowerUp(WeaponType weaponType)
{
	WeaponInfo* weaponInfo = WeaponInfoManager::GetInstance().GetWeaponInfo(weaponType);
	auto it = weapons.find(weaponType);

	if (weaponInfo && it != weapons.end())
	{
		WeaponInfo* weapon = it->second;

		if (weaponInfo->numberOfClips != INFINITE_CLIPS)
			weapon->numberOfClips += weaponInfo->numberOfClips;

		this->LoadFirstClip(weapon);
	}
}

void Weapon::LoadFirstClip(WeaponInfo* weapon)
{
	if (!weapon)
		return;

	bool infiniteClips = weapon->numberOfClips == INFINITE_CLIPS || globalInfo.infinite_clips;

	// if there are no bullets loaded but you have some clips to load
	if (weapon->bulletsLoaded == 0 && (infiniteClips || weapon->numberOfClips > 0))
	{
		weapon->bulletsLoaded = weapon->clipSize;

		if (weapon->numberOfClips != INFINITE_CLIPS)
			weapon->numberOfClips--;
	}
}

// method uses both globalInfo and weaponInfo
BulletInfo Weapon::ComputeFinalBulletInfo(const BulletInfo& bulletInfo) const
{
	BulletInfo finalBulletInfo = bulletInfo;
	finalBulletInfo.bulletSpeed *= globalInfo.bulletSpeedProc;
	finalBulletInfo.damage += globalInfo.extraDamage;
	finalBulletInfo.range += globalInfo.extraRange;
	finalBulletInfo.pushBack &= globalInfo.pushBack;
	
	//finalBulletInfo.pushBackPower /= globalInfo.rateOfFireProc;
	return finalBulletInfo;
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE