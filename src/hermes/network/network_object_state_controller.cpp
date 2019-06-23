#include "hermes/network/network_object_state_controller.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

NetworkObjectStateController::NetworkObjectStateController(NetworkObjectStateId networkObjectStateId)
	: m_networkObjectStateId(networkObjectStateId)
{
}

NetworkObjectStateId NetworkObjectStateController::GetNetworkObjectStateId() const
{
	return this->m_networkObjectStateId;
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE