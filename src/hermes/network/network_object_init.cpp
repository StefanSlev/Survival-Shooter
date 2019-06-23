#include "hermes/network/network_object_init.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

NetworkObjectInitWrapper::NetworkObjectInitWrapper() :
	m_messageReadStream(yojimbo::GetDefaultAllocator(), m_messageData, MaxMessageSize),
	m_messageWriteStream(yojimbo::GetDefaultAllocator(), m_messageData, MaxMessageSize)
{
	memset(m_messageData, 0, sizeof(NetworkMessageData));
}

yojimbo::ReadStream& NetworkObjectInitWrapper::GetMessageReadStream()
{
	return this->m_messageReadStream;
}

yojimbo::WriteStream& NetworkObjectInitWrapper::GetMessageWriteStream()
{
	return this->m_messageWriteStream;
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE