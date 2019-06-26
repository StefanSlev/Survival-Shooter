#ifndef HERMES_NETWORK_OBJECT_STATE
#define HERMES_NETWORK_OBJECT_STATE

#include "hermes/network/network_base.h"
#include "hermes/network/network_message.h"
#include "hermes/network/network_message_type.h"
#include "hermes/network/network_channel_type.h"
#include "hermes/network/network_object_id.h"
#include "hermes/network/network_object_state_data.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class NetworkObjectState final : public NetworkMessage
{
public:
	NetworkMessageType GetNetworkMessageType() const final;
	NetworkChannelType GetNetworkChannelType() const final;

	NetworkObjectIdValue GetNetworkObjectIdValue() const;
	void SetNetworkObjectIdValue(NetworkObjectIdValue objectIdValue);

	NetworkObjectStateId GetNetworkObjectStateId() const;
	void SetNetworkObjectStateId(NetworkObjectStateId objectStateId);

	NetworkObjectStateIndex GetNetworkObjectStateIndex() const;
	void SetNetworkObjectStateIndex(NetworkObjectStateIndex objectStateIndex);

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		if (!NetworkMessage::Serialize(stream))
		{
			return false;
		}

		serialize_uint64(stream, m_networkObjectIdValue);
		serialize_uint32(stream, m_networkObjectStateId);
		serialize_uint64(stream, m_networkObjectStateIndex);

		return true;
	}

	NetworkObjectIdValue m_networkObjectIdValue;
	NetworkObjectStateId m_networkObjectStateId;
	NetworkObjectStateIndex m_networkObjectStateIndex;
};

using NetworkObjectStateUPtr = std::unique_ptr<NetworkObjectState>;
using NetworkObjectStateSPtr = std::shared_ptr<NetworkObjectState>;

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_OBJECT_STATE
