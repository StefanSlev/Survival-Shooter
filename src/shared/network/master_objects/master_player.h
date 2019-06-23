#ifndef SHARED_NETWORK_MASTER_PLAYER
#define SHARED_NETWORK_MASTER_PLAYER

#include "shared/network/network_base.h"
#include "shared/gameplay/elements/actors/player.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class MasterPlayer final : public gameplay::Player, public hermes::network::MasterObject
{
public:
	MasterPlayer(hermes::network::NetworkObjectId networkObjectId);
	hermes::network::NetworkObjectInitDataSPtr GetNetworkObjectInit() const override;
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_MASTER_PLAYER