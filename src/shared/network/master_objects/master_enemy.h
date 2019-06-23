#ifndef SHARED_NETWORK_MASTER_ENEMY
#define SHARED_NETWORK_MASTER_ENEMY

#include "shared/network/network_base.h"
#include "shared/gameplay/elements/actors/enemy.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class MasterEnemy : public gameplay::Enemy, public hermes::network::MasterObject
{
public:
	MasterEnemy(hermes::network::NetworkObjectId networkObjectId);
	hermes::network::NetworkObjectInitDataSPtr GetNetworkObjectInit() const override;

	void Init() override;
	void Update(float dt) override;

private:
	// timer for follow
	float waitForFollow;
	// the time it has to wait before resetting its target
	const float followTime = 2.f;

	gameplay::Actor* GetTarget() const;
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_MASTER_ENEMY