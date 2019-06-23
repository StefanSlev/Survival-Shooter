#include "shared/gameplay/elements/power_ups/power_up_controller.h"
#include "shared/gameplay/elements/power_ups/power_up_manager.h"
#include "shared/gameplay/elements/utilities/countdown_clock.h"
#include "shared/gameplay/elements/actors/actor.h"
#include "shared/gameplay/engine/collider.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

PowerUpController::PowerUpController()
{
	this->actor = nullptr;
}

void PowerUpController::SetObject(Object * _object)
{
	Controller::SetObject(_object);

	if (_object->IsTypeOf(ObjectType::Actor))
		this->actor = static_cast<Actor*> (_object);
}

void PowerUpController::ClearPowerUps()
{
	for (auto it = powerUps.begin(); it != powerUps.end(); ++it)
	{
		PowerUpType powerUpType = it->first;
		CountdownClock* countdownClock = it->second;

		PowerUpManager& powerUpManager = PowerUpManager::GetInstance();
		PowerUpEffect* powerUpEffect = powerUpManager.GetPowerUpEffect(powerUpType);

		// deactivate the effect of the powerUp
		if (powerUpEffect != nullptr && powerUpEffect->GetKind() == PowerUpKind::Expirable)
		{
			static_cast<ExpirablePowerUpEffect*> (powerUpEffect)->handler(actor, false);
		}

		// remove the clock from the player and destroy it
		countdownClock->Remove();
		delete countdownClock;
	}

	powerUps.clear();
}

void PowerUpController::Update(float dt)
{
	for (auto it = powerUps.begin(); it != powerUps.end(); ++it)
	{
		// PowerUpType powerUpType = it->first;
		CountdownClock* countdownClock = it->second;

		// if time is up for that powerUp
		if (countdownClock->HasEnded())
		{
			/*GameEvent* removePowerUpEvent = new RemovePowerUpEvent(actor->GetID(), powerUpType);
			Game::g_pGame->gameEventsQueue->PushEvent(removePowerUpEvent);*/
		}
	}

	Controller::Update(dt);
}

void PowerUpController::ApplyPowerUp(PowerUpType powerUpType)
{
	PowerUpManager& powerUpManager = PowerUpManager::GetInstance();
	PowerUpEffect* powerUpEffect = powerUpManager.GetPowerUpEffect(powerUpType);

	if (powerUpEffect == nullptr || powerUpEffect->GetKind() == PowerUpKind::Permanent)
		return;

	ExpirablePowerUpEffect* expirableEffect = static_cast<ExpirablePowerUpEffect*> (powerUpEffect);

	auto it = powerUps.find(powerUpType);

	// if powerUpType is not applied on the actor
	if (it == powerUps.end())
	{
		// create a clock to show the remained time of the effect 
		CountdownClock* countdownClock = new CountdownClock();
		countdownClock->LoadClockIcon(expirableEffect->powerUpSprite);
		countdownClock->SetTime(expirableEffect->duration);
		
		// add the clock on the actor
		actor->AddChild(countdownClock);

		// activate the effect of the powerUp
		expirableEffect->handler(actor, true);

		// add the new record and refresh clocks' position
		powerUps[powerUpType] = countdownClock;
		this->RefreshClocks();
	}
	else it->second->AddTime(expirableEffect->duration);
}

void PowerUpController::RemovePowerUp(PowerUpType powerUpType)
{
	bool shouldRefresh = false;

	auto it = powerUps.find(powerUpType);

	// if powerUpType is applied on the actor
	if (it != powerUps.end())
	{
		CountdownClock* countdownClock = it->second;
		shouldRefresh = true;

		PowerUpManager& powerUpManager = PowerUpManager::GetInstance();
		PowerUpEffect* powerUpEffect = powerUpManager.GetPowerUpEffect(powerUpType);

		// deactivate the effect of the powerUp
		if (powerUpEffect != nullptr && powerUpEffect->GetKind() == PowerUpKind::Expirable)
		{
			static_cast<ExpirablePowerUpEffect*> (powerUpEffect)->handler(actor, false);
		}

		// remove the clock from the player and destroy it
		countdownClock->Remove();
		delete countdownClock;

		it = powerUps.erase(it);
	}

	if (shouldRefresh)
		this->RefreshClocks();
}

void PowerUpController::RefreshClocks()
{
	// refresh clocks' positions
	int nrClocks = static_cast<int> (powerUps.size());

	if (nrClocks)
	{
		CountdownClock* clock = powerUps.begin()->second;

		float clockRadius = fmaxf(clock->size.x, clock->size.y) / 2;
		
		// calculate clock's X position
		int index = (nrClocks & 1) ? nrClocks / 2 : nrClocks - 1;

		float clockPosX = -index * clockRadius;
		clockPosX = (nrClocks & 1) ? clockPosX * 2.f : clockPosX;

		// calculate clock's Y position
		float clockPosY = -actor->collider->radius * 1.5f - clockRadius;

		for (auto it = powerUps.begin(); it != powerUps.end(); ++it)
		{
			it->second->position = Vector2D(clockPosX, clockPosY);

			// update X position for the next clock
			clockPosX += clockRadius * 2.f;
		}
	}
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE