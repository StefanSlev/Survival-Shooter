#ifndef SHARED_NETWORK_PLAYER_INIT_DATA
#define SHARED_NETWORK_PLAYER_INIT_DATA

#include "shared/network/network_base.h"
#include "shared/gameplay/engine/vector2d.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class PlayerInitData : public hermes::network::NetworkObjectInitData
{
public:
	PlayerInitData();
	
	// Transform2D data
	gameplay::Vector2D m_position;
	float m_rotation;
	int m_life;

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()
	
private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		serialize_float(stream, m_position.x);
		serialize_float(stream, m_position.y);
		serialize_float(stream, m_rotation);
		serialize_int(stream, m_life, 0, 100);
		return true;
	}
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_PLAYER_INIT_DATA