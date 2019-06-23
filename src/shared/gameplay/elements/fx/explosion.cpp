#include "shared/gameplay/elements/fx/explosion.h"
#include "shared/gameplay/elements/game_events/game_event.h"
#include "shared/gameplay/elements/game_events/game_events_queue.h"
#include "shared/gameplay/elements/game.h"

#include "shared/gameplay/engine/collider.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

Explosion::Explosion() : PhysicsObject(nullptr)
{
	spriteAnimation = new SpriteAnimationController<ExplosionAnimation>();
	this->AddController(spriteAnimation);
}

void Explosion::Load(int loadIdGenerator, int loadDamage)
{
	this->idGenerator = loadIdGenerator;
	this->damage = loadDamage;
}

void Explosion::Init()
{
	PhysicsObject::Init();

	this->collider->isTrigger = true;
	
	damage = 0;
	idGenerator = -1;

	hitIDs.clear();
	spriteAnimation->ClearAnimation();
}

void Explosion::OnCollisionTrigger(PhysicsObject* pOther)
{
	auto it = find_if(hitIDs.begin(), hitIDs.end(), [pOther](int objectID) {

		return (pOther->GetID() == objectID);
	});

	if (it == hitIDs.end() && ResolveCollision(pOther))
	{
		// generate damage event
		GameEvent* gameEvent = new DamageActorEvent(idGenerator, pOther->GetID(), damage);
		Game::g_pGame->gameEventsQueue->PushEvent(gameEvent);

		hitIDs.push_back(pOther->GetID());
	}
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE