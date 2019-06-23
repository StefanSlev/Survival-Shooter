#include "shared/gameplay/elements/power_ups/power_up.h"
#include "shared/gameplay/elements/power_ups/power_up_manager.h"
#include "shared/gameplay/elements/game_events/game_event.h"
#include "shared/gameplay/elements/game_events/game_events_queue.h"
#include "shared/gameplay/elements/game.h"

#include "shared/gameplay/engine/collider.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

PowerUp::PowerUp() : PhysicsObject(nullptr) 
{
}

void PowerUp::Init()
{
	PhysicsObject::Init();

	// from Object
	size = 2;

	// from PhysicsObject
	collider->ComputeDefaultRadius();
	collider->isTrigger = true;

	// from PowerUp
	lifeTime = totalLifeTime;
	triggerID = -1;
}

void PowerUp::Load(PowerUpType loadPowerUpType)
{
	this->powerUpType = loadPowerUpType;

	// load sprite for current powerUp
	PowerUpManager& powerUpManager = PowerUpManager::GetInstance();
	PowerUpEffect* powerUpEffect = powerUpManager.GetPowerUpEffect(loadPowerUpType);

	this->pSprite = powerUpEffect->powerUpSprite;
}

void PowerUp::Update(float dt)
{
	if (!active)
		return;

	if (lifeTime == 0.f)
	{
		GameEvent* endGameObjectEvent = new EndGameObjectEvent(GetID());
		Game::g_pGame->gameEventsQueue->PushEvent(endGameObjectEvent);

		return;
	}
	
	if (lifeTime < totalBlinkTime)
	{
		// how much a blink lasts
		float blinkTime = totalBlinkTime / (2 * nrBlinks + 1);
		float progress = (totalBlinkTime - lifeTime) / blinkTime;
		float time = progress - (int) progress;

		time = ((int)progress % 2 == 0) ? 1 - time : time;

		colorMask = LerpColor(transparentMask, visibleMask, time);
	}

	lifeTime = fmaxf(lifeTime - dt, 0.f);
}

void PowerUp::OnEnd()
{
	// look for trigger
	Object* trigger = Object::FindByID(triggerID);

	if (trigger && trigger->IsTypeOf(ObjectType::Player))
	{
		// create applyPowerUpEvent 
	/*	GameEvent* applyPowerUpEvent = new ApplyPowerUpEvent(triggerID, powerUpType);
		Game::g_pGame->gameEventsQueue->PushEvent(applyPowerUpEvent);*/

		// reset triggerID
		triggerID = -1;
	}

	PhysicsObject::OnEnd();
}

void PowerUp::OnCollisionTrigger(PhysicsObject* pOther)
{
	if (pOther->IsTypeOf(ObjectType::Player) && triggerID == -1) 
	{
		triggerID = pOther->GetID();

		GameEvent* endGameObjectEvent = new EndGameObjectEvent(GetID());
		Game::g_pGame->gameEventsQueue->PushEvent(endGameObjectEvent);
	}
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE