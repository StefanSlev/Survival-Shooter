#ifndef SHARED_NETWORK_MASTER_KAMIKAZE
#define SHARED_NETWORK_MASTER_KAMIKAZE

#include "shared/network/network_base.h"
#include "shared/network/master_objects/master_enemy.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class MasterKamikaze final : public MasterEnemy
{
public:
	MasterKamikaze(hermes::network::NetworkObjectId networkObjectId);
	
	ObjectType GetType() const override { return ObjectType::Kamikaze; }
	bool IsTypeOf(ObjectType type) const override { return (type == ObjectType::Kamikaze) || MasterEnemy::IsTypeOf(type); }

	void Init() override;
	void OnEnd() override;

private:
	void HandleAttack() override;
	static const char* walkAnim;
	static const char* deathAnim;
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_MASTER_KAMIKAZE