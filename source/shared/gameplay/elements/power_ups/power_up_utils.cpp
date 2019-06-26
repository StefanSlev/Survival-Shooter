#include "shared/gameplay/elements/power_ups/power_up_utils.h"
#include "shared/gameplay/elements/power_ups/power_up_info.h"

#include "shared/gameplay/elements/fx/nuke_explosion.h"
#include "shared/gameplay/elements/weapons/weapon.h"
#include "shared/gameplay/elements/actors/player.h"
#include "shared/gameplay/elements/game.h"

#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/rigidbody.h"
#include "shared/gameplay/engine/collider.h"
#include "shared/gameplay/engine/camera.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

void PowerUpUtils::HandleWeapon(Actor* actor, WeaponType weaponType)
{
	if (actor->IsTypeOf(ObjectType::Player))
	{
		Player* player = static_cast<Player*> (actor);
		player->weapon->ApplyWeaponPowerUp(weaponType);
		player->weapon->ChangeWeapon(weaponType);
	}
}

void PowerUpUtils::HandleAssault(Actor* actor)
{
	HandleWeapon(actor, WeaponType::AssaultRifle);
}

void PowerUpUtils::HandleRocket(Actor* actor)
{
	HandleWeapon(actor, WeaponType::RocketLauncher);
}

void PowerUpUtils::HandleGauss(Actor* actor)
{
	HandleWeapon(actor, WeaponType::GaussRifle);
}

void PowerUpUtils::HandleInfiniteAmmo(Actor* actor, bool _switch)
{
	if (actor->IsTypeOf(ObjectType::Player))
	{
		Player* player = static_cast<Player*> (actor);
		player->weapon->globalInfo.infinite_ammo = _switch;
	}
}

void PowerUpUtils::HandleFireRate(Actor* actor, bool _switch)
{
	if (actor->IsTypeOf(ObjectType::Player))
	{
		Player* player = static_cast<Player*> (actor);

		float rateOfFireProc = _switch ? 2.f : 1.f;
		player->weapon->globalInfo.rateOfFireProc = rateOfFireProc;
	}
}

void PowerUpUtils::HandlePushBack(Actor* actor, bool _switch)
{
	if (actor->IsTypeOf(ObjectType::Player))
	{
		Player* player = static_cast<Player*> (actor);
		player->weapon->globalInfo.pushBack = _switch;
	}
}

const char* PowerUpUtils::shieldFileName = "Data\\Images\\shield.png";
const char* PowerUpUtils::freezeFileName = "Data\\Images\\freeze.png";

void PowerUpUtils::HandleInvulnerable(Actor* actor, bool _switch)
{
	actor->invulnerable = _switch;

	// add shield
	if (_switch && actor->shield == nullptr)
	{
		WorldObject* shield = new WorldObject();
		shield->pSprite = ResourceManager::GetInstance().GetSprite(ResourceType::IMAGE, shieldFileName);
		shield->size = actor->collider->radius * 3.f;

		actor->shield = shield;
		actor->AddChild(shield);
	}
	else if (!_switch && actor->shield != nullptr)
	{
		actor->shield->Remove();
		SAFE_DELETE(actor->shield);
	}
}

void PowerUpUtils::HandleFreeze(Actor* actor, bool _switch)
{
	if (_switch)
	{
		actor->UpdateState(ActorState::INACTIVE);
		actor->rigidbody->velocity = 0.f;
		actor->spriteAnimation->Pause();

		// add freeze object
		WorldObject* freeze = new WorldObject();
		freeze->pSprite = ResourceManager::GetInstance().GetSprite(ResourceType::IMAGE, freezeFileName);
		freeze->size = actor->collider->radius * 2.f;

		actor->freeze = freeze;
		actor->AddChild(freeze);
	}
	else {

		actor->UpdateState(actor->GetPrevState());
		actor->spriteAnimation->Continue();

		if (actor->freeze != nullptr)
		{
			actor->freeze->Remove();
			SAFE_DELETE(actor->freeze);
		}
	}
}

void PowerUpUtils::HandleEnemiesFreeze(Actor* actor)
{
	UNREFERENCED_PARAMETER(actor);

	auto enemies = Game::g_pGame->enemies;

	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		if ((*it)->IsAlive())
		{
			/*GameEvent* applyPowerUpEvent = new ApplyPowerUpEvent(enemy->GetID(), PowerUpType::Freeze);
			Game::g_pGame->gameEventsQueue->PushEvent(applyPowerUpEvent);*/
		}
	}
}

void PowerUpUtils::HandleNuke(Actor* actor)
{
	NukeExplosion* nukeExplosion = new NukeExplosion();

	Vector2D nukePos = 0.f;
	Camera2D::g_pActiveCamera->WorldSpaceToVirtualScreenSpace(actor->position, nukePos);
	
	nukeExplosion->position = nukePos;
	nukeExplosion->Init(actor->GetID());

	Game::g_pGame->AddGameObject(nukeExplosion);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE