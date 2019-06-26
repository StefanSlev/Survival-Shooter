#include "shared/gameplay/elements/game_events/game_event.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

GameEvent* GameEvent::BuildGameEvent(hermes::network::NetworkEventSPtr networkEvent)
{
	GameEventBuildHelper buildHelper;

	BEGIN_NETWORK_MESSAGE_READING(networkEvent, eventStream)
		buildHelper.SerializeInternal(eventStream);
	END_NETWORK_MESSAGE_READING(networkEvent, eventStream)
	
	GameEventType gameEventType = static_cast<GameEventType> (buildHelper.gameEventTypeIndx);
	
	auto& gameEventFactories = GameEventFactories::gameEventFactories();

	auto it_factories = gameEventFactories.find(gameEventType);

	if (it_factories != gameEventFactories.end())
	{
		auto& gameEventFactory = it_factories->second;
		GameEvent* gameEvent = gameEventFactory();

		BEGIN_NETWORK_MESSAGE_READING(networkEvent, eventStream)
			gameEvent->SerializeInternal(eventStream);
		END_NETWORK_MESSAGE_READING(networkEvent, eventStream)

		return gameEvent;
	}

	return nullptr;
}

DamageActorEvent::DamageActorEvent(int idGenerator, int idTarget, int damage) : GameEvent(GameEventType::DamageActor) 
{
	this->idGenerator = idGenerator;
	this->idTarget = idTarget;
	this->damage = damage;
}

BulletShotEvent::BulletShotEvent(int idGenerator, Vector2D position, float rotation, BulletType bulletType) : GameEvent(GameEventType::BulletShot)
{
	this->idGenerator = idGenerator;
	this->position = position;
	this->rotation = rotation;
	this->bulletType = bulletType;
}

EndGameObjectEvent::EndGameObjectEvent(int idGameObject) : GameEvent(GameEvent::GameEventType::EndGameObject)
{
	this->idGameObject = idGameObject;
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE