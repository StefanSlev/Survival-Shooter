#ifndef HERMES_NETWORK_WORKER
#define HERMES_NETWORK_WORKER

#include "hermes/network/network_base.h"
#include "hermes/network/network_message.h"
#include "hermes/system/singleton.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class NetworkDispatcher;

class NetworkWorker final : public system::Singleton<NetworkWorker>
{
	SINGLETON_ACCESS(NetworkWorker)

public:
	void PushNetworkMessage(NetworkMessageSPtr networkMessage);
	void OnNetworkMessageReceived(NetworkMessageSPtr networkMessage);

	bool RegisterNetworkDispatcher(NetworkDispatcher* networkDispatcher);
	bool UnregisterNetworkDispatcher(NetworkDispatcher* networkDispatcher);

private:
	NetworkDispatcher* m_networkDispatcher;

	NetworkWorker();
	~NetworkWorker() override {}
};

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#define GetNetworkWorker() hermes::network::NetworkWorker::GetInstance()

#endif // HERMES_NETWORK_WORKER