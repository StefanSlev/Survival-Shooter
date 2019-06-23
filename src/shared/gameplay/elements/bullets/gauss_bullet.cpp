#include "shared/gameplay/elements/bullets/gauss_bullet.h"
#include "shared/gameplay/elements/game_events/game_event.h"
#include "shared/gameplay/elements/game_events/game_events_queue.h"
#include "shared/gameplay/elements/game.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

GaussBullet::GaussBullet() : Bullet() {}

void GaussBullet::Init()
{
	Bullet::Init();

	hitIDs.clear();
}

void GaussBullet::OnEnd()
{
	PhysicsObject::OnEnd();
}

void GaussBullet::OnCollisionTrigger(PhysicsObject* pOther)
{
	auto it = find_if(hitIDs.begin(), hitIDs.end(), [pOther](int objectID) {

		return (pOther->GetID() == objectID);
	});

	if (it == hitIDs.end() && ResolveCollision(pOther))
	{
		hitIDs.push_back(pOther->GetID());

		// generate damage event
		GameEvent* gameEvent = new DamageActorEvent(idGenerator, pOther->GetID(), bulletInfo->damage);
		Game::g_pGame->gameEventsQueue->PushEvent(gameEvent);
	}
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE