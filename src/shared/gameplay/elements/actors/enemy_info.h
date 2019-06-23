#ifndef SHARED_GAMEPLAY_ELEMENTS_ENEMY_INFO
#define SHARED_GAMEPLAY_ELEMENTS_ENEMY_INFO

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/update_list.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

const ObjectType enemyTypes[] = {
									ObjectType::Cyclope,
									ObjectType::Ghoul,
									ObjectType::Kamikaze,
									ObjectType::Spider,
									ObjectType::Undead
};

const size_t nrEnemies = sizeof(enemyTypes) / sizeof(enemyTypes[0]);

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ELEMENTS_ENEMY_INFO