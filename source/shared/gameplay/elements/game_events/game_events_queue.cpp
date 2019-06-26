#include "shared/gameplay/elements/game_events/game_events_queue.h"
#include "shared/gameplay/elements/game_utilities/scoring_system.h"
#include "shared/gameplay/elements/actors/actor.h"
#include "shared/gameplay/elements/actors/enemy.h"
#include "shared/gameplay/elements/bullets/bullet.h"
#include "shared/gameplay/elements/bullets/gauss_bullet.h"
#include "shared/gameplay/elements/bullets/rocket.h"
#include "shared/gameplay/elements/weapons/weapon_info_manager.h"
#include "shared/gameplay/elements/game.h"
#include "shared/gameplay/engine/object_pools.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

GameEventsQueue::GameEventsQueue()
{
	this->UpdateRights(Rights::Master);
}

void GameEventsQueue::UpdateRights(Rights updateRights)
{
	this->rights = updateRights;

	switch (rights)
	{
	case Rights::Master:
	{
		for (unsigned int i = 0; i < nrGameEventTypes; i++)
		{
			GameEvent::GameEventType gameEventType = gameEventTypes[i];
			gameEventRights[gameEventType] = true;
		}
		break;
	}
	case Rights::Slave:
	{
		for (unsigned int i = 0; i < nrGameEventTypes; i++)
		{
			GameEvent::GameEventType gameEventType = gameEventTypes[i];
			gameEventRights[gameEventType] = false;
		}

		gameEventRights[GameEvent::GameEventType::BulletShot] = true;
		break;
	}
	default:
		break;
	}
}

GameEventsQueue::Rights GameEventsQueue::GetRights() const
{
	return this->rights;
}

Subject& GameEventsQueue::GameEventNotify()
{
	return gameEventNotify;
}

void GameEventsQueue::PushEvent(GameEvent* newGameEvent)
{
	gameEvents.push(newGameEvent);
}

void GameEventsQueue::ProcessEvents()
{
	Game::GameState gameState = Game::g_pGame->gameState;

	while (!gameEvents.empty())
	{
		GameEvent* gameEvent = gameEvents.front();

		// if the queue is not master over a certain event,
		// it won't handle it, instead the queue drops it
		if (gameState == Game::GameState::InGame && gameEventRights[gameEvent->gameEventType])
		{
			bool eventHandled = this->HandleEvent(gameEvent);

			// notify the observers only if an event occured(was handled properly)
			if (eventHandled)
				gameEventNotify.NotifyObservers(gameEvent);
		}

		delete gameEvent;
		gameEvents.pop();
	}
}

bool GameEventsQueue::HandleEvent(GameEvent* gameEvent)
{
	auto it_handler = gameEventHandlers.find(gameEvent->gameEventType);

	if (it_handler != gameEventHandlers.end())
	{
		GameEventHandler gameEventHandler = it_handler->second;

		return gameEventHandler(*this, gameEvent);
	}

	return false;
}

// Internal methods
bool GameEventsQueue::HandleDamageActorEvent(GameEvent* gameEvent)
{
	if (gameEvent->gameEventType != GameEvent::GameEventType::DamageActor)
		return false;

	DamageActorEvent* damageActorEvent = static_cast<DamageActorEvent*> (gameEvent);

	int idGenerator = damageActorEvent->idGenerator;
	int idTarget = damageActorEvent->idTarget;
	int damage = damageActorEvent->damage;

	Object* target = Object::FindByID(idTarget);

	if (target == nullptr || !target->IsTypeOf(ObjectType::Actor))
		return false;

	Actor* targetActor = static_cast<Actor*> (target);

	targetActor->TakeDamage(damage);

	// handle actor death and scoring events
	Object* generator = Object::FindByID(idGenerator);

	if (targetActor->IsDead() && targetActor->IsTypeOf(ObjectType::Enemy) && 
		generator && generator->IsTypeOf(ObjectType::Player))
	{
		int score = static_cast<Enemy*> (targetActor)->GetScore();
		GetScoringSystem().AddScoreForPlayer(idGenerator, score);
	}

	return true;
}

bool GameEventsQueue::HandleBulletShotEvent(GameEvent* gameEvent)
{
	if (gameEvent->gameEventType != GameEvent::GameEventType::BulletShot)
		return false;

	BulletShotEvent* damageActorEvent = static_cast<BulletShotEvent*> (gameEvent);

	int idGenerator = damageActorEvent->idGenerator;
	Vector2D position = damageActorEvent->position;
	float rotation = damageActorEvent->rotation;
	BulletType bulletType = damageActorEvent->bulletType;

	Bullet* bullet = nullptr;
	ObjectPools& objectPools = ObjectPools::GetInstance();

	switch (bulletType)
	{
	case BulletType::GunBullet:
		bullet = static_cast<Bullet*> (objectPools.GetObjectFromPool(ObjectType::Bullet));
		break;
	case BulletType::AssaultBullet:
		bullet = static_cast<Bullet*> (objectPools.GetObjectFromPool(ObjectType::Bullet));
		break;
	case BulletType::GaussBullet:
		bullet = static_cast<GaussBullet*> (objectPools.GetObjectFromPool(ObjectType::GaussBullet));
		break;
	case BulletType::Rocket:
		bullet = static_cast<Rocket*> (objectPools.GetObjectFromPool(ObjectType::Rocket));
		break;
	default:
		break;
	}

	if (bullet)
	{
		WeaponInfoManager& weaponInfoManager = WeaponInfoManager::GetInstance();
		BulletInfo* bulletInfo = weaponInfoManager.GetBulletInfo(bulletType);

		bullet->LoadBulletInfo(*bulletInfo);
		bullet->Load(position, rotation);
		bullet->idGenerator = idGenerator;

		Game::g_pGame->AddGameObject(bullet);
		return true;
	}

	return false;
}

bool GameEventsQueue::HandleEndGameObjectEvent(GameEvent* gameEvent)
{
	if (gameEvent->gameEventType != GameEvent::GameEventType::EndGameObject)
		return false;

	EndGameObjectEvent* endGameObjectEvent = static_cast<EndGameObjectEvent*> (gameEvent);

	int idGameObject = endGameObjectEvent->idGameObject;

	Object* gameObject = Object::FindByID(idGameObject);

	if (gameObject != nullptr && gameObject->IsAlive())
	{
		gameObject->End();
		return true;
	}

	return false;
}

void GameEventsQueue::ClearEvents()
{
	while (!gameEvents.empty())
	{
		delete gameEvents.front();

		gameEvents.pop();
	}
}

GameEventsQueue::~GameEventsQueue()
{
	this->ClearEvents();
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE