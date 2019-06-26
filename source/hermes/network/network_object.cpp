#include "hermes/network/network_object.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

NetworkObject::NetworkObject(NetworkObjectId networkObjectId) : m_networkObjectId(networkObjectId)
{
}

const NetworkObjectId& NetworkObject::GetNetworkObjectId() const
{
	return this->m_networkObjectId;
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE