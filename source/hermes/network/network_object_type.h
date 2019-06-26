#ifndef HERMES_NETWORK_OBJECT_TYPE
#define HERMES_NETWORK_OBJECT_TYPE

#include "hermes/network/network_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

enum class NetworkObjectType : std::uint8_t
{
	Master,
	Replica
};

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_OBJECT_TYPE
