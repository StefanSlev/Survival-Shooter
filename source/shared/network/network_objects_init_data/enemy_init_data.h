#ifndef SHARED_NETWORK_ENEMY_INIT_DATA
#define SHARED_NETWORK_ENEMY_INIT_DATA

#include "shared/network/network_base.h"
#include "shared/gameplay/engine/vector2d.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class EnemyInitData : public hermes::network::NetworkObjectInitData
{
public:
	EnemyInitData();

	// Transform2D data
	gameplay::Vector2D m_position;
	int m_life;

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		serialize_float(stream, m_position.x);
		serialize_float(stream, m_position.y);
		serialize_int(stream, m_life, -1, 1000);
		return true;
	}
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_ENEMY_INIT_DATA