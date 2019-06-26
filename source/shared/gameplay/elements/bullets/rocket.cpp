#include "shared/gameplay/elements/bullets/rocket.h"
#include "shared/gameplay/elements/fx/rocket_explosion.h"
#include "shared/gameplay/elements/game.h"
#include "shared/gameplay/engine/object_pools.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

Rocket::Rocket() : Bullet() {}

void Rocket::OnEnd()
{
	Object* rocketExplosion = ObjectPools::GetInstance().GetObjectFromPool(ObjectType::RocketExplosion);
	static_cast<Explosion*> (rocketExplosion)->Load(idGenerator, bulletInfo->damage);
	
	rocketExplosion->position = position;

	Game::g_pGame->AddGameObject(rocketExplosion);
	
	PhysicsObject::OnEnd();
}

bool Rocket::ResolveCollision(PhysicsObject* pOther)
{
	return pOther->IsTypeOf(ObjectType::Enemy);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE