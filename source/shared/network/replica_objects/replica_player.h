#ifndef SHARED_NETWORK_REPLICA_PLAYER
#define SHARED_NETWORK_REPLICA_PLAYER

#include "shared/network/network_base.h"
#include "shared/gameplay/elements/actors/player.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class ReplicaPlayer : public gameplay::Player, public hermes::network::ReplicaObject
{
public:
	ReplicaPlayer(hermes::network::NetworkObjectId networkObjectId);
	hermes::network::NetworkObjectInitDataSPtr GetNetworkObjectInit() const override;
	void OnNetworkObjectInit(hermes::network::NetworkObjectInitWrapper& objectInitWrapper) override;

	void Update(float dt) override;
private:

};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_REPLICA_PLAYER