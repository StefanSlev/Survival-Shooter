#ifndef SHARED_NETWORK_OBJECT_ROTATION_DATA
#define SHARED_NETWORK_OBJECT_ROTATION_DATA

#include "shared/network/network_base.h"

BEGIN_SHARED_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class NetworkObjectRotationData : public hermes::network::NetworkObjectStateData
{
public:
	NetworkObjectRotationData()
	{
		m_rotation = 0.f;
	}

	float m_rotation;
	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		serialize_float(stream, m_rotation);
		return true;
	}
};

END_NETWORK_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_NETWORK_OBJECT_ROTATION_DATA