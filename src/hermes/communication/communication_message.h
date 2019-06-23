#ifndef HERMES_COMMUNICATION_MESSAGE
#define HERMES_COMMUNICATION_MESSAGE

#include "hermes/communication/communication_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_COMMUNICATION_NAMESPACE

class CommunicationMessage : public yojimbo::Message
{
public:
	CommunicationMessage();
	yojimbo::ReadStream& GetMessageReadStream();
	yojimbo::WriteStream& GetMessageWriteStream();

	double GetTimestamp() const;
	void SetTimestamp(double timestamp);
	
	std::uint32_t GetMessageType() const;
	void SetMessageType(std::uint32_t messageType);

	template <class Stream> bool Serialize(Stream& stream)
	{
		serialize_double(stream, m_timestamp);
		serialize_uint32(stream, m_messageType);

		int messageDataBytes = 0;

		if (Stream::IsWriting)
			messageDataBytes = m_messageWriteStream.GetBytesProcessed();

		serialize_int(stream, messageDataBytes, 0, MaxMessageSize);
		serialize_bytes(stream, m_messageData, messageDataBytes);
		return true;
	}

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	double m_timestamp;
	std::uint32_t m_messageType;

	yojimbo::ReadStream m_messageReadStream;
	yojimbo::WriteStream m_messageWriteStream;
	CommunicationMessageData m_messageData;
};

enum class CommunicationMessageType : std::uint8_t
{
	CommunicationMessage
};

END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE

#define BEGIN_COMMUNICATION_MESSAGE_WRITING(communicationMessage, writeStream)                   \
    do                                                                                           \
    {                                                                                            \
        yojimbo::WriteStream& writeStream = communicationMessage->GetMessageWriteStream();       \

#define END_COMMUNICATION_MESSAGE_WRITING(communicationMessage, writeStream)                     \
        writeStream.Flush();                                                                     \
    }                                                                                            \
    while(0);

#define BEGIN_COMMUNICATION_MESSAGE_READING(communicationMessage, readStream)                    \
    do                                                                                           \
    {                                                                                            \
        yojimbo::ReadStream& readStream = communicationMessage->GetMessageReadStream();          \

#define END_COMMUNICATION_MESSAGE_READING(communicationMessage, readStream)                      \
    }                                                                                            \
    while(0);

#define GetCommunicationMessageIndex(communicationMessageType) static_cast<int> (communicationMessageType)

#endif // HERMES_COMMUNICATION_MESSAGE