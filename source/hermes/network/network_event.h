#ifndef HERMES_NETWORK_EVENT
#define HERMES_NETWORK_EVENT

#include "hermes/network/network_base.h"
#include "hermes/network/network_message.h"
#include "hermes/network/network_message_type.h"
#include "hermes/network/network_channel_type.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class NetworkEvent final : public NetworkMessage
{
public:
	NetworkMessageType GetNetworkMessageType() const final;
	NetworkChannelType GetNetworkChannelType() const final;

	std::uint32_t GetNetworkEventType() const;
	void SetNetworkEventType(std::uint32_t networkEventType);

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		if (!NetworkMessage::Serialize(stream))
		{
			return false;
		}

		serialize_uint32(stream, m_networkEventType);
		return true;
	}

	std::uint32_t m_networkEventType;
};

using NetworkEventUPtr = std::unique_ptr<NetworkEvent>;
using NetworkEventSPtr = std::shared_ptr<NetworkEvent>;

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_EVENT
