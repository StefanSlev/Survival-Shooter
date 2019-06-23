#include "hermes/network/network_object_id.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

NetworkObjectId::NetworkObjectId() : m_networkId(0), m_localObjectId(0) 
{
}

NetworkId NetworkObjectId::GetNetworkId() const
{
	return this->m_networkId;
}

ObjectId NetworkObjectId::GetLocalObjectId() const
{
	return this->m_localObjectId;
}

NetworkObjectIdValue NetworkObjectId::GetValue() const
{
	return (static_cast<std::uint64_t>(m_networkId) << 32) | m_localObjectId;
}

void NetworkObjectId::SetValue(NetworkId networkId, ObjectId localObjectId)
{
	this->m_networkId = networkId;
	this->m_localObjectId = localObjectId;
}

void NetworkObjectId::SetValue(NetworkObjectIdValue networkObjectIdValue)
{
	this->m_networkId = (networkObjectIdValue >> 32) & UINT32_MAX;
	this->m_localObjectId = networkObjectIdValue & UINT32_MAX;
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE