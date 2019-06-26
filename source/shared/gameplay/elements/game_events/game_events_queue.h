#ifndef SHARED_GAMEPLAY_ELEMENTS_GAME_EVENTS_QUEUE
#define SHARED_GAMEPLAY_ELEMENTS_GAME_EVENTS_QUEUE

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/elements/game_events/game_event.h"
#include "shared/gameplay/engine/subject.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class GameEventsQueue
{
public:
	enum class Rights
	{
		Master,
		Slave
	};

	GameEventsQueue();

	void UpdateRights(Rights updateRights);
	Rights GetRights() const;

	void PushEvent(GameEvent* newGameEvent);
	Subject& GameEventNotify();
	
	// returns true if event was handled properly
	bool HandleEvent(GameEvent* gameEvent);
	void ProcessEvents();

	void ClearEvents();
	~GameEventsQueue();

private:
	using GameEventHandler = std::function<bool(GameEventsQueue&, GameEvent*)>;

	const std::unordered_map<GameEvent::GameEventType, GameEventHandler> gameEventHandlers = {

		{GameEvent::GameEventType::DamageActor, &GameEventsQueue::HandleDamageActorEvent},
		{GameEvent::GameEventType::BulletShot, &GameEventsQueue::HandleBulletShotEvent},
		{GameEvent::GameEventType::EndGameObject, &GameEventsQueue::HandleEndGameObjectEvent},
	};

	std::unordered_map<GameEvent::GameEventType, bool> gameEventRights;

	bool HandleDamageActorEvent(GameEvent* gameEvent);
	bool HandleBulletShotEvent(GameEvent* gameEvent);
	bool HandleEndGameObjectEvent(GameEvent* gameEvent);

	std::queue<GameEvent*> gameEvents;
	Rights rights;
	Subject gameEventNotify;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_GAME_EVENTS_QUEUE