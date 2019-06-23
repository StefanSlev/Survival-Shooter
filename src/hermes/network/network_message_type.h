#ifndef HERMES_NETWORK_MESSAGE_TYPE
#define HERMES_NETWORK_MESSAGE_TYPE

#include "hermes/network/network_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

enum class NetworkMessageType : std::uint8_t
{
	NetworkObjectState,
	NetworkEvent,
};

#define GetNetworkMessageIndex(networkMessageType) static_cast<int> (networkMessageType)

constexpr NetworkMessageType networkMessageTypes[] = {

	NetworkMessageType::NetworkObjectState,
	NetworkMessageType::NetworkEvent
};

constexpr int networkMessagesCount = sizeof(networkMessageTypes) / sizeof(NetworkMessageType);

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_MESSAGE_TYPE
