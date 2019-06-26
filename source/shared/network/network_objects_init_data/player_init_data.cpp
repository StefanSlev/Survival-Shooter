#include "shared/network/network_objects_init_data/player_init_data.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

PlayerInitData::PlayerInitData()
{
	m_position = 0.f;
	m_rotation = 0.f;
	m_life = 0;
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE