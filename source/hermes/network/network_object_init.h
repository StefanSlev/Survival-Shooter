#ifndef HERMES_NETWORK_OBJECT_INIT
#define HERMES_NETWORK_OBJECT_INIT

#include "hermes/network/network_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

using NetworkObjectInitData = yojimbo::Serializable;
using NetworkObjectInitDataUPtr = std::unique_ptr<yojimbo::Serializable>;
using NetworkObjectInitDataSPtr = std::shared_ptr<yojimbo::Serializable>;

class NetworkObjectInitWrapper final : public yojimbo::Serializable
{
public:
	NetworkObjectInitWrapper();
	yojimbo::ReadStream& GetMessageReadStream();
	yojimbo::WriteStream& GetMessageWriteStream();

	YOJIMBO_VIRTUAL_SERIALIZE_FUNCTIONS()

private:
	template <class Stream> bool Serialize(Stream& stream)
	{
		int messageDataBytes = 0;

		if (Stream::IsWriting)
			messageDataBytes = m_messageWriteStream.GetBytesProcessed();

		serialize_int(stream, messageDataBytes, 0, MaxMessageSize);
		serialize_bytes(stream, m_messageData, messageDataBytes);
		return true;
	}

	yojimbo::ReadStream m_messageReadStream;
	yojimbo::WriteStream m_messageWriteStream;
	NetworkMessageData m_messageData;
};


END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#define BEGIN_OBJECT_INIT_DATA_WRITING(objectInitWrapper, writeStream)                   \
    do                                                                                   \
    {                                                                                    \
        yojimbo::WriteStream& writeStream = objectInitWrapper.GetMessageWriteStream();   \

#define END_OBJECT_INIT_DATA_WRITING(objectInitWrapper, writeStream)                     \
        writeStream.Flush();                                                             \
    }                                                                                    \
    while(0);

#define BEGIN_OBJECT_INIT_DATA_READING(objectInitWrapper, readStream)                    \
    do                                                                                   \
    {                                                                                    \
        yojimbo::ReadStream& readStream = objectInitWrapper.GetMessageReadStream();      \

#define END_OBJECT_INIT_DATA_READING(objectInitWrapper, readStream)                      \
    }                                                                                    \
    while(0);

#endif // HERMES_NETWORK_OBJECT_INIT