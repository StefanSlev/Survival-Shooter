#ifndef HERMES_NETWORK_CHANNEL_TYPE
#define HERMES_NETWORK_CHANNEL_TYPE

#include "hermes/network/network_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

enum class NetworkChannelType : std::uint8_t
{
	ReliableOrdered,
	UnreliableUnordered
};

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_CHANNEL_TYPE
