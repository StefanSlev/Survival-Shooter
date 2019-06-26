#ifndef SHARED_NETWORK_REPLICA_UNDEAD
#define SHARED_NETWORK_REPLICA_UNDEAD

#include "shared/network/network_base.h"
#include "shared/network/replica_objects/replica_enemy.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class ReplicaUndead final : public ReplicaEnemy
{
public:
	ReplicaUndead(hermes::network::NetworkObjectId networkObjectId);

	ObjectType GetType() const override { return ObjectType::Undead; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Undead) || ReplicaEnemy::IsTypeOf(type); }

	void Init() override;

private:
	static const char* walkAnim;
	static const char* deathAnim;
	static const char* attackAnim;
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_REPLICA_UNDEAD