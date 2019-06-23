#ifndef SHARED_NETWORK_OBJECT_POSITION_STATE_DATA
#define SHARED_NETWORK_OBJECT_POSITION_STATE_DATA

#include "shared/network/network_base.h"
#include "shared/gameplay/engine/vector2d.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class NetworkObjectPositionData : public hermes::network::NetworkObjectStateData
{
public:
	NetworkObjectPositionData()
	{
		m_position = 0.f;
	}

	gameplay::Vector2D m_position;
	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		serialize_float(stream, m_position.x);
		serialize_float(stream, m_position.y);
		return true;
	}
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_OBJECT_POSITION_STATE_DATA