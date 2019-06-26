#include "shared/network/master_objects/master_player.h"
#include "shared/network/network_objects_init_data/player_init_data.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

MasterPlayer::MasterPlayer(hermes::network::NetworkObjectId networkObjectId)
	: MasterObject(networkObjectId)
{
	Player::Init();
}

hermes::network::NetworkObjectInitDataSPtr MasterPlayer::GetNetworkObjectInit() const
{
	auto playerInitData = std::make_shared<PlayerInitData>();
	
	playerInitData->m_position = position;
	playerInitData->m_rotation = rotation;
	playerInitData->m_life = life;

	return std::static_pointer_cast<hermes::network::NetworkObjectInitData>(playerInitData);
}

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE