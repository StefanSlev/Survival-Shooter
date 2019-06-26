#ifndef HERMES_NETWORK_MESSAGE
#define HERMES_NETWORK_MESSAGE

#include "hermes/network/network_base.h"
#include "hermes/network/network_message_type.h"
#include "hermes/network/network_channel_type.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class NetworkMessage : public yojimbo::Serializable
{
public:
	yojimbo::ReadStream& GetMessageReadStream();
	yojimbo::WriteStream& GetMessageWriteStream();

	virtual NetworkMessageType GetNetworkMessageType() const = 0;
	virtual NetworkChannelType GetNetworkChannelType() const = 0;

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

protected:
	NetworkMessage();

	template <class Stream> bool Serialize(Stream& stream)
	{
		int messageDataBytes = 0;

		if (Stream::IsWriting)
			messageDataBytes = m_messageWriteStream.GetBytesProcessed();

		serialize_int(stream, messageDataBytes, 0, MaxMessageSize);

		if (Stream::IsReading)
		{
			NetworkMessageData auxData;
			memset(auxData, 0, sizeof(NetworkMessageData));

			serialize_bytes(stream, auxData, messageDataBytes);
			serialize_bytes(m_messageWriteStream, auxData, messageDataBytes);
			m_messageWriteStream.Flush();
		}
		else
		{
			serialize_bytes(stream, m_messageData, messageDataBytes);
		}

		return true;
	}

private:
	yojimbo::ReadStream m_messageReadStream;
	yojimbo::WriteStream m_messageWriteStream;
	NetworkMessageData m_messageData;
};

using NetworkMessageUPtr = std::unique_ptr<NetworkMessage>;
using NetworkMessageSPtr = std::shared_ptr<NetworkMessage>;

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#define BEGIN_NETWORK_MESSAGE_WRITING(networkMessage, writeStream)                   \
    do                                                                               \
    {                                                                                \
        yojimbo::WriteStream& writeStream = networkMessage->GetMessageWriteStream(); \

#define END_NETWORK_MESSAGE_WRITING(networkMessage, writeStream)                     \
        writeStream.Flush();                                                         \
    }                                                                                \
    while(0);

#define BEGIN_NETWORK_MESSAGE_READING(networkMessage, readStream)                    \
    do                                                                               \
    {                                                                                \
        yojimbo::ReadStream& readStream = networkMessage->GetMessageReadStream();    \

#define END_NETWORK_MESSAGE_READING(networkMessage, readStream)                      \
    }                                                                                \
    while(0);

#endif // HERMES_NETWORK_MESSAGE