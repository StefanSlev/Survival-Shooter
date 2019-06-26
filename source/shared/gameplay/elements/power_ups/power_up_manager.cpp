#include "shared/gameplay/elements/power_ups/power_up_manager.h"
#include "shared/gameplay/elements/power_ups/power_up_utils.h"
#include "shared/gameplay/engine/resource_manager.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const char* PowerUpManager::assault_path = "Data\\Images\\powerup_assault.png";
const char* PowerUpManager::gauss_path = "Data\\Images\\powerup_gauss.png";
const char* PowerUpManager::rocket_path = "Data\\Images\\powerup_rocket.png";
const char* PowerUpManager::infinite_ammo_path = "Data\\Images\\powerup_infiniteammo.png";
const char* PowerUpManager::fire_rate_path = "Data\\Images\\powerup_firerate.png";
const char* PowerUpManager::push_back_path = "Data\\Images\\powerup_pushback.png";
const char* PowerUpManager::invulnerable_path = "Data\\Images\\powerup_invulnerable.png";
const char* PowerUpManager::freeze_path = "Data\\Images\\powerup_freeze.png";
const char* PowerUpManager::nuke_path = "Data\\Images\\powerup_nuke.png";

PowerUpManager::~PowerUpManager()
{
	for (auto it = powerUpEffects.begin(); it != powerUpEffects.end(); it++)
		delete it->second;
}

PowerUpEffect* PowerUpManager::GetPowerUpEffect(PowerUpType powerUpType)
{
	// get the effect asociated with the given powerUpType
	auto it = powerUpEffects.find(powerUpType);

	if (it != powerUpEffects.end())
		return it->second;

	return nullptr;
}

PowerUpManager::PowerUpManager()
{
	// load infos for weapons
	ResourceManager& resourceManager = ResourceManager::GetInstance();

	// assault ammo
	PermanentPowerUpEffect* assault = new PermanentPowerUpEffect();
	assault->powerUpType = PowerUpType::Assault;
	assault->powerUpSprite = resourceManager.GetSprite(ResourceType::IMAGE, assault_path);
	assault->handler = &PowerUpUtils::HandleAssault;

	// gauss ammo
	PermanentPowerUpEffect* gauss = new PermanentPowerUpEffect();
	gauss->powerUpType = PowerUpType::Gauss;
	gauss->powerUpSprite = resourceManager.GetSprite(ResourceType::IMAGE, gauss_path);
	gauss->handler = &PowerUpUtils::HandleGauss;

	// rocket ammo
	PermanentPowerUpEffect* rocket = new PermanentPowerUpEffect();
	rocket->powerUpType = PowerUpType::Rocket;
	rocket->powerUpSprite = resourceManager.GetSprite(ResourceType::IMAGE, rocket_path);
	rocket->handler = &PowerUpUtils::HandleRocket;

	// infinite_ammo
	ExpirablePowerUpEffect* infiniteAmmo = new ExpirablePowerUpEffect();
	infiniteAmmo->powerUpType = PowerUpType::InfiniteAmmo;
	infiniteAmmo->powerUpSprite = resourceManager.GetSprite(ResourceType::IMAGE, infinite_ammo_path);
	infiniteAmmo->duration = 15.f;
	infiniteAmmo->handler = &PowerUpUtils::HandleInfiniteAmmo;

	// increase fire_rate
	ExpirablePowerUpEffect* fireRate = new ExpirablePowerUpEffect();
	fireRate->powerUpType = PowerUpType::FireRate;
	fireRate->powerUpSprite = resourceManager.GetSprite(ResourceType::IMAGE, fire_rate_path);
	fireRate->duration = 15.f;
	fireRate->handler = &PowerUpUtils::HandleFireRate;

	// push_back ammo
	ExpirablePowerUpEffect* pushBack = new ExpirablePowerUpEffect();
	pushBack->powerUpType = PowerUpType::PushBack;
	pushBack->powerUpSprite = resourceManager.GetSprite(ResourceType::IMAGE, push_back_path);
	pushBack->duration = 15.f;
	pushBack->handler = &PowerUpUtils::HandlePushBack;
	
	// invulnerable
	ExpirablePowerUpEffect* invulnerable = new ExpirablePowerUpEffect();
	invulnerable->powerUpType = PowerUpType::Invulnerable;
	invulnerable->powerUpSprite = resourceManager.GetSprite(ResourceType::IMAGE, invulnerable_path);
	invulnerable->duration = 15.f;
	invulnerable->handler = &PowerUpUtils::HandleInvulnerable;

	// freeze
	ExpirablePowerUpEffect* freeze = new ExpirablePowerUpEffect();
	freeze->powerUpType = PowerUpType::Freeze;
	freeze->powerUpSprite = resourceManager.GetSprite(ResourceType::IMAGE, freeze_path);
	freeze->duration = 15.f;
	freeze->handler = &PowerUpUtils::HandleFreeze;

	// freeze enemies
	PermanentPowerUpEffect* enemiesFreeze = new PermanentPowerUpEffect();
	enemiesFreeze->powerUpType = PowerUpType::EnemiesFreeze;
	enemiesFreeze->powerUpSprite = resourceManager.GetSprite(ResourceType::IMAGE, freeze_path);
	enemiesFreeze->handler = &PowerUpUtils::HandleEnemiesFreeze;

	// nuke
	PermanentPowerUpEffect* nuke = new PermanentPowerUpEffect();
	nuke->powerUpType = PowerUpType::Nuke;
	nuke->powerUpSprite = resourceManager.GetSprite(ResourceType::IMAGE, nuke_path);
	nuke->handler = &PowerUpUtils::HandleNuke;

	powerUpEffects[PowerUpType::Assault] = assault;
	powerUpEffects[PowerUpType::Gauss] = gauss;
	powerUpEffects[PowerUpType::Rocket] = rocket;
	powerUpEffects[PowerUpType::InfiniteAmmo] = infiniteAmmo;
	powerUpEffects[PowerUpType::FireRate] = fireRate;
	powerUpEffects[PowerUpType::PushBack] = pushBack;
	powerUpEffects[PowerUpType::Invulnerable] = invulnerable;
	powerUpEffects[PowerUpType::Freeze] = freeze;
	powerUpEffects[PowerUpType::EnemiesFreeze] = enemiesFreeze;
	powerUpEffects[PowerUpType::Nuke] = nuke;
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE