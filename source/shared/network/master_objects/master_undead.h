#ifndef SHARED_NETWORK_MASTER_UNDEAD
#define SHARED_NETWORK_MASTER_UNDEAD

#include "shared/network/network_base.h"
#include "shared/network/master_objects/master_enemy.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class MasterUndead final : public MasterEnemy
{
public:
	MasterUndead(hermes::network::NetworkObjectId networkObjectId);
	
	ObjectType GetType() const override { return ObjectType::Undead; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Undead) || MasterEnemy::IsTypeOf(type); }

	void Init() override;

private:
	static const char* walkAnim;
	static const char* deathAnim;
	static const char* attackAnim;
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_MASTER_UNDEAD