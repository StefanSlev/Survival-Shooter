#include "hermes/network/network_event_data.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

std::uint32_t NetworkFlowInitializeEventData::GetNetworkId()
{
	return this->m_networkId;
}

void NetworkFlowInitializeEventData::SetNetworkId(std::uint32_t networkId)
{
	this->m_networkId = networkId;
}

std::uint32_t CreateReplicaObjectEventData::GetObjectType() const
{
	return this->m_objectType;
}

void CreateReplicaObjectEventData::SetObjectType(std::uint32_t objectType)
{
	this->m_objectType = objectType;
}

NetworkObjectIdValue CreateReplicaObjectEventData::GetNetworkObjectIdValue() const
{
	return this->m_networkObjectIdValue;
}

void CreateReplicaObjectEventData::SetNetworkObjectIdValue(NetworkObjectIdValue objectIdValue)
{
	this->m_networkObjectIdValue = objectIdValue;
}

bool CreateReplicaObjectEventData::HasNetworkObjectInitData() const
{
	return this->m_hasNetworkObjectInitData;
}

void CreateReplicaObjectEventData::SetHasNetworkObjectInitData(bool hasNetworkObjectInitData)
{
	this->m_hasNetworkObjectInitData = hasNetworkObjectInitData;
}

NetworkObjectInitWrapper& CreateReplicaObjectEventData::GetNetworkObjectInitWrapper()
{
	return this->m_networkObjectInitWrapper;
}

NetworkObjectIdValue DestroyReplicaObjectEventData::GetNetworkObjectIdValue() const
{
	return this->m_networkObjectIdValue;
}

void DestroyReplicaObjectEventData::SetNetworkObjectIdValue(NetworkObjectIdValue objectIdValue)
{
	this->m_networkObjectIdValue = objectIdValue;
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE