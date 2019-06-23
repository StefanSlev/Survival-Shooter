#include "shared/network/master_objects/master_enemy.h"
#include "shared/network/network_objects_init_data/enemy_init_data.h"
#include "shared/gameplay/elements/game.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

MasterEnemy::MasterEnemy(hermes::network::NetworkObjectId networkObjectId) 
	: MasterObject(networkObjectId)
{
}

hermes::network::NetworkObjectInitDataSPtr MasterEnemy::GetNetworkObjectInit() const
{
	auto enemyInitData = std::make_shared<EnemyInitData>();

	enemyInitData->m_position = position;
	enemyInitData->m_rotation = rotation;
	enemyInitData->m_life = life;

	return std::static_pointer_cast<hermes::network::NetworkObjectInitData>(enemyInitData);
}

void MasterEnemy::Init()
{
	Enemy::Init();

	waitForFollow = 0.f;
}

void MasterEnemy::Update(float dt)
{
	if (IsAlive())
	{
		if (!target || !target->IsAlive() || waitForFollow == 0.f)
		{
			target = this->GetTarget();

			changedTarget = target ? true : false;
			waitForFollow = followTime;
		}
		else 
			changedTarget = false;

		waitForFollow = fmaxf(0.f, waitForFollow - dt);
	}

	Enemy::Update(dt);
}

gameplay::Actor* MasterEnemy::GetTarget() const
{
	const auto& players = gameplay::Game::g_pGame->players;

	Actor *newTarget = nullptr;
	float minDist = 0.f;
	bool foundAny = false;

	for (unsigned int i = 0; i < players.size(); ++i)
	{
		Actor* player = static_cast<Actor*> (players[i]);

		if (player && player->IsAlive())
		{
			float currentDist = DistSq(position, player->position);

			if (!foundAny || minDist > currentDist)
			{
				minDist = currentDist;
				newTarget = player;
				foundAny = true;
			}
		}
	}

	return newTarget;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE