#ifndef SHARED_GAMEPLAY_ELEMENTS_POWER_UP_INFO
#define SHARED_GAMEPLAY_ELEMENTS_POWER_UP_INFO

#include "shared/gameplay/gameplay_base.h"

// -------------------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------------------

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

enum class PowerUpType
{
	Assault,
	Gauss,
	Rocket,
	InfiniteAmmo,
	FireRate,
	PushBack,
	Invulnerable,
	Freeze,
	EnemiesFreeze,
	Nuke
};

enum class PowerUpKind
{
	Expirable,
	Permanent
};

// Freeze PowerUp is missing from the array because this one
// is accesible through "EnemiesFreeze powerUp"

const PowerUpType powerUpTypes[] = { 
										PowerUpType::Assault,
										PowerUpType::Gauss,
										PowerUpType::Rocket,
										PowerUpType::InfiniteAmmo,
										PowerUpType::FireRate,
										PowerUpType::PushBack,
										PowerUpType::Invulnerable,
										PowerUpType::EnemiesFreeze,
										PowerUpType::Nuke
									};

const PowerUpType weaponPowerUpTypes[] = {
											PowerUpType::Assault,
											PowerUpType::Gauss,
											PowerUpType::Rocket,
										 };

const size_t nrPowerUps = sizeof(powerUpTypes) / sizeof(powerUpTypes[0]);
const size_t nrWeaponPowerUps = sizeof(weaponPowerUpTypes) / sizeof(weaponPowerUpTypes[0]);

// PowerUpEffect 

class Sprite;
class Actor;

struct PowerUpEffect
{
	PowerUpType powerUpType;
	Sprite* powerUpSprite;

	PowerUpKind GetKind() const {
		return this->powerUpKind;
	}
protected:
	PowerUpEffect(PowerUpKind powerUpKind) : powerUpKind(powerUpKind)
	{
		powerUpSprite = nullptr;
	}
private:
	PowerUpKind powerUpKind;
};

struct PermanentPowerUpEffect final : public PowerUpEffect
{
	PermanentPowerUpEffect() : PowerUpEffect(PowerUpKind::Permanent)
	{
		handler = nullptr;
	}

	std::function<void(Actor*)> handler;
};

struct ExpirablePowerUpEffect final : public PowerUpEffect
{
	ExpirablePowerUpEffect() : PowerUpEffect(PowerUpKind::Expirable)
	{
		duration = 0.f;
		handler = nullptr;
	}

	float duration;
	std::function<void(Actor*, bool)> handler;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_POWER_UP_INFO