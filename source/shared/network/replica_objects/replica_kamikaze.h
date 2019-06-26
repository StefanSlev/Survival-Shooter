#ifndef SHARED_NETWORK_REPLICA_KAMIKAZE
#define SHARED_NETWORK_REPLICA_KAMIKAZE

#include "shared/network/network_base.h"
#include "shared/network/replica_objects/replica_enemy.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class ReplicaKamikaze final : public ReplicaEnemy
{
public:
	ReplicaKamikaze(hermes::network::NetworkObjectId networkObjectId);
	
	ObjectType GetType() const override { return ObjectType::Kamikaze; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Kamikaze) || ReplicaEnemy::IsTypeOf(type); }

	void Init() override;
	void OnEnd() override;

private:
	static const char* walkAnim;
	static const char* deathAnim;
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_REPLICA_KAMIKAZE