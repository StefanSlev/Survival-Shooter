#ifndef SHARED_NETWORK_ENEMY_TARGET_DATA
#define SHARED_NETWORK_ENEMY_TARGET_DATA

#include "shared/network/network_base.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class EnemyTargetData : public hermes::network::NetworkObjectStateData
{
public:
	EnemyTargetData()
	{
		m_objectIdValue = hermes::network::InvalidNetworkObjectIdValue;
	}

	hermes::network::NetworkObjectIdValue m_objectIdValue;
	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		serialize_uint64(stream, m_objectIdValue);
		return true;
	}
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_ENEMY_TARGET_DATA
