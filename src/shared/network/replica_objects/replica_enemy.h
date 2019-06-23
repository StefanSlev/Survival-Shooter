#ifndef SHARED_NETWORK_REPLICA_ENEMY
#define SHARED_NETWORK_REPLICA_ENEMY

#include "shared/network/network_base.h"
#include "shared/gameplay/elements/actors/enemy.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class ReplicaEnemy : public gameplay::Enemy, public hermes::network::ReplicaObject
{
public:
	ReplicaEnemy(hermes::network::NetworkObjectId networkObjectId);
	hermes::network::NetworkObjectInitDataSPtr GetNetworkObjectInit() const override;
	void OnNetworkObjectInit(hermes::network::NetworkObjectInitWrapper& objectInitWrapper) override;

private:

};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_REPLICA_ENEMY