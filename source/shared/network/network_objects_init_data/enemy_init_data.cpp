#include "shared/network/network_objects_init_data/enemy_init_data.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

EnemyInitData::EnemyInitData()
{
	m_position = 0.f;
	m_life = -1;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE