#ifndef HERMES_COMMUNICATION_DEDICATED_SERVER_ADAPTER
#define HERMES_COMMUNICATION_DEDICATED_SERVER_ADAPTER

#include "hermes/communication/communication_base.h"
#include "hermes/communication/communication_adapter.h"
#include "hermes/communication/dedicated_server/dedicated_server_callbacks.h"

BEGIN_HERMES_NAMESPACE

BEGIN_COMMUNICATION_NAMESPACE

class DedicatedServerAdapter : public CommunicationAdapter
{
public:
	DedicatedServerAdapter(DedicatedServerCallbacks* dedicatedServerCallbacks = nullptr);

	void SetDedicatedServerCallbacks(DedicatedServerCallbacks* dedicatedServerCallbacks);

	void OnServerClientConnected(int clientIndex) final;
	void OnServerClientDisconnected(int clientIndex) final;

private:
	DedicatedServerCallbacks* m_dedicatedServerCallbacks;
};

END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_COMMUNICATION_DEDICATED_SERVER_ADAPTER