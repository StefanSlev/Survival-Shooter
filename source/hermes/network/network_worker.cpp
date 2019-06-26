#include "hermes/network/network_worker.h"
#include "hermes/network/network_dispatcher.h"
#include "hermes/network/network_message_type.h"
#include "hermes/network/network_object_state.h"
#include "hermes/network/network_event.h"
#include "hermes/network/network_registry.h"
#include "hermes/network/network_flow.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

NetworkWorker::NetworkWorker()
{
	this->m_networkDispatcher = nullptr;
}

void NetworkWorker::OnNetworkMessageReceived(NetworkMessageSPtr networkMessage)
{
	if (!networkMessage)
		return;

	switch (networkMessage->GetNetworkMessageType())
	{
		case NetworkMessageType::NetworkObjectState:
		{
			NetworkObjectStateSPtr networkObjectState = std::static_pointer_cast<NetworkObjectState> (networkMessage);
			GetNetworkRegistry().OnObjectStateReceived(networkObjectState);
			break;
		}
		case NetworkMessageType::NetworkEvent:
		{
			NetworkEventSPtr networkEvent = std::static_pointer_cast<NetworkEvent> (networkMessage);
			GetNetworkFlow().OnEventReceived(networkEvent);
			break;
		}
		default:
			break;
	}
}

void NetworkWorker::PushNetworkMessage(NetworkMessageSPtr networkMessage)
{
	if (!m_networkDispatcher || !networkMessage)
		return;

	m_networkDispatcher->DispatchNetworkMessage(networkMessage);
}

bool NetworkWorker::RegisterNetworkDispatcher(NetworkDispatcher* networkDispatcher)
{
	if (!networkDispatcher)
		return false;

	if (m_networkDispatcher == nullptr)
	{
		m_networkDispatcher = networkDispatcher;
		return true;
	}

	return false;
}

bool NetworkWorker::UnregisterNetworkDispatcher(NetworkDispatcher* networkDispatcher)
{
	if (!networkDispatcher)
		return false;

	if (m_networkDispatcher == networkDispatcher)
	{
		m_networkDispatcher = nullptr;
		return true;
	}

	return false;
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE