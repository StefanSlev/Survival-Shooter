#include "shared/gameplay/elements/fx/nuke_explosion.h"
#include "shared/gameplay/elements/game_events/game_event.h"
#include "shared/gameplay/elements/game_events/game_events_queue.h"
#include "shared/gameplay/elements/game.h"

#include "shared/gameplay/engine/resource_manager.h"
#include "shared/gameplay/engine/constants.h"
#include "shared/gameplay/engine/camera.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const char* NukeExplosion::nukeFileName = "Data\\Images\\nuke.png";

NukeExplosion::NukeExplosion() : MenuItem()
{
	pSprite = ResourceManager::GetInstance().GetSprite(ResourceType::IMAGE, nukeFileName);
	size = 0.f;
	lifeTime = 0.f;
	idGenerator = -1;
}

void NukeExplosion::Update(float dt)
{
	if (!active)
		return;
	
	if (Game::g_pGame->gameState == Game::GameState::InGame)
	{
		if (lifeTime == totalLifeTime)
		{
			End();
			return;
		}

		float finalLifeTime = growthFactor * lifeTime;
		float proc = (lifeTime * 2.f <= totalLifeTime) ? finalLifeTime : (float) pow(M_EULER, pow(finalLifeTime, 2.5f));

		size = proc * Camera2D::g_pActiveCamera->virtualScreenSize;

		lifeTime = fminf(lifeTime + dt, totalLifeTime);
	}
}

void NukeExplosion::OnEnd()
{
	auto enemies = Game::g_pGame->enemies;

	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		Object* enemy = (*it);

		if (enemy->IsAlive())
		{
			// generate damage event
			GameEvent* gameEvent = new DamageActorEvent(idGenerator, enemy->GetID(), nukeDamage);
			Game::g_pGame->gameEventsQueue->PushEvent(gameEvent);
		}
	}

	MenuItem::OnEnd();
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE