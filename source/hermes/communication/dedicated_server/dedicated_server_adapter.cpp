#include "hermes/communication/dedicated_server/dedicated_server_adapter.h"

BEGIN_HERMES_NAMESPACE

BEGIN_COMMUNICATION_NAMESPACE

DedicatedServerAdapter::DedicatedServerAdapter(DedicatedServerCallbacks* dedicatedServerCallbacks) 
	: m_dedicatedServerCallbacks(dedicatedServerCallbacks)
{
}

void DedicatedServerAdapter::SetDedicatedServerCallbacks(DedicatedServerCallbacks* dedicatedServerCallbacks)
{
	this->m_dedicatedServerCallbacks = dedicatedServerCallbacks;
}

void DedicatedServerAdapter::OnServerClientConnected(int clientIndex)
{
	if (m_dedicatedServerCallbacks)
		m_dedicatedServerCallbacks->OnClientConnected(clientIndex);
}

void DedicatedServerAdapter::OnServerClientDisconnected(int clientIndex)
{
	if (m_dedicatedServerCallbacks)
		m_dedicatedServerCallbacks->OnClientDisconnected(clientIndex);
}

END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE