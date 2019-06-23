#include "hermes/communication/communication_message.h"

BEGIN_HERMES_NAMESPACE

BEGIN_COMMUNICATION_NAMESPACE

CommunicationMessage::CommunicationMessage() :
	m_messageReadStream(yojimbo::GetDefaultAllocator(), m_messageData, MaxMessageSize),
	m_messageWriteStream(yojimbo::GetDefaultAllocator(), m_messageData, MaxMessageSize)
{
	memset(m_messageData, 0, sizeof(CommunicationMessageData));
}

yojimbo::ReadStream& CommunicationMessage::GetMessageReadStream()
{
	return this->m_messageReadStream;
}

yojimbo::WriteStream& CommunicationMessage::GetMessageWriteStream()
{
	return this->m_messageWriteStream;
}

double CommunicationMessage::GetTimestamp() const
{
	return this->m_timestamp;
}

void CommunicationMessage::SetTimestamp(double timestamp)
{
	this->m_timestamp = timestamp;
}

std::uint32_t CommunicationMessage::GetMessageType() const
{
	return this->m_messageType;
}

void CommunicationMessage::SetMessageType(std::uint32_t messageType)
{
	this->m_messageType = messageType;
}

END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE