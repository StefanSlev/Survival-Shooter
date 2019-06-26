#include "hermes/network/network_event.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

NetworkMessageType NetworkEvent::GetNetworkMessageType() const
{
	return NetworkMessageType::NetworkEvent;
}

NetworkChannelType NetworkEvent::GetNetworkChannelType() const
{
	return NetworkChannelType::ReliableOrdered;
}

std::uint32_t NetworkEvent::GetNetworkEventType() const
{
	return this->m_networkEventType;
}

void NetworkEvent::SetNetworkEventType(std::uint32_t networkEventType)
{
	this->m_networkEventType = networkEventType;
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE