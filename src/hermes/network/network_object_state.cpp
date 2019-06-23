#include "hermes/network/network_object_state.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

NetworkMessageType NetworkObjectState::GetNetworkMessageType() const
{
	return NetworkMessageType::NetworkObjectState;
}

NetworkChannelType NetworkObjectState::GetNetworkChannelType() const
{
	return NetworkChannelType::UnreliableUnordered;
}

NetworkObjectIdValue NetworkObjectState::GetNetworkObjectIdValue() const
{
	return this->m_networkObjectIdValue;
}

void NetworkObjectState::SetNetworkObjectIdValue(NetworkObjectIdValue networkObjectIdValue)
{
	this->m_networkObjectIdValue = networkObjectIdValue;
}

NetworkObjectStateId NetworkObjectState::GetNetworkObjectStateId() const
{
	return this->m_networkObjectStateId;
}

void NetworkObjectState::SetNetworkObjectStateId(NetworkObjectStateId networkObjectStateId)
{
	this->m_networkObjectStateId = networkObjectStateId;
}

NetworkObjectStateIndex NetworkObjectState::GetNetworkObjectStateIndex() const
{
	return this->m_networkObjectStateIndex;
}

void NetworkObjectState::SetNetworkObjectStateIndex(NetworkObjectStateIndex networkObjectStateIndex)
{
	this->m_networkObjectStateIndex = networkObjectStateIndex;
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE