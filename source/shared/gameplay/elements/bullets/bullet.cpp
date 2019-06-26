#include "shared/gameplay/elements/bullets/bullet.h"
#include "shared/gameplay/elements/fx/bullet_explosion.h"
#include "shared/gameplay/elements/game_events/game_event.h"
#include "shared/gameplay/elements/game_events/game_events_queue.h"
#include "shared/gameplay/elements/game.h"

#include "shared/gameplay/engine/collider.h"
#include "shared/gameplay/engine/rigidbody.h"
#include "shared/gameplay/engine/object_pools.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

Bullet::Bullet() : PhysicsObject(nullptr) {

	this->bulletInfo = new BulletInfo();
}

void Bullet::Init()
{
	PhysicsObject::Init();

	this->initialPosition = 0;
	this->idGenerator = -1;
	this->collider->isTrigger = true;
}

Bullet::~Bullet()
{
	if (bulletInfo)
		delete bulletInfo;
}

void Bullet::LoadBulletInfo(const BulletInfo& loadBulletInfo)
{
	*this->bulletInfo = loadBulletInfo;
	this->pSprite = loadBulletInfo.bulletSprite;
	this->size = loadBulletInfo.size;
	this->color = loadBulletInfo.color;
	this->collider->ComputeDefaultRadius();
}

void Bullet::Load(Vector2D loadPosition, float loadRotation)
{
	// set bullet rotation
	this->rotation = loadRotation;

	// set bullet velocity
	Vector2D bulletVelocity = { 0,1 };
	bulletVelocity = bulletVelocity.Rotate(loadRotation);

	// set bullet position
	this->position = loadPosition + bulletVelocity * bulletInfo->barrelOffset;
	initialPosition = this->position;

	rigidbody->velocity = bulletVelocity * bulletInfo->bulletSpeed;
}

void Bullet::Update(float dt)
{
	if (!active)
		return;

	const float eps = 1e-6f;

	// check for bullet reaching its range
	if (DistSq(initialPosition, position) - bulletInfo->range * bulletInfo->range > -eps)
	{
		End();
		return;
	}

	PhysicsObject::Update(dt);
}

void Bullet::OnEnd()
{
	// ending animation
	Object* bulletExplosion = ObjectPools::GetInstance().GetObjectFromPool(ObjectType::BulletExplosion);
	bulletExplosion->position = position;

	Game::g_pGame->AddGameObject(bulletExplosion);

	PhysicsObject::OnEnd();
}

bool Bullet::ResolveCollision(PhysicsObject* pOther)
{	
	bool collisionEvent = false;
	
	// check enemy collision
	collisionEvent |= pOther->IsTypeOf(ObjectType::Enemy);

	if (collisionEvent)
	{
		if (bulletInfo->pushBack)
		{
			Vector2D bulletDirection = Vector2D(0, 1).Rotate(rotation);
			pOther->rigidbody->AddImpulseForTime(bulletDirection * bulletInfo->pushBackPower, 0.2f);
		}

		return true;
	}

	return false;
}

void Bullet::OnCollisionTrigger(PhysicsObject* pOther)
{
	if (this->ResolveCollision(pOther))
	{
		// generate damage event
		GameEvent* gameEvent = new DamageActorEvent(idGenerator, pOther->GetID(), bulletInfo->damage);
		Game::g_pGame->gameEventsQueue->PushEvent(gameEvent);

		End();
	}
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE