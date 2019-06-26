#ifndef HERMES_COMMUNICATION_DEDICATED_SERVER
#define HERMES_COMMUNICATION_DEDICATED_SERVER

#include "hermes/communication/communication_base.h"
#include "hermes/communication/dedicated_server/dedicated_server_callbacks.h"
#include "hermes/communication/dedicated_server/dedicated_server_adapter.h"
// Network
#include "hermes/network/network_message.h"
#include "hermes/network/network_dispatcher.h"
#include "hermes/network/network_event.h"
#include "hermes/network/replica_object.h"
#include "hermes/network/network_object_id.h"
#include "hermes/network/network_flow_listener.h"

BEGIN_HERMES_NAMESPACE

BEGIN_COMMUNICATION_NAMESPACE

class CommunicationMessage;
class GameClient;

class DedicatedServer :
	public yojimbo::Server,
	public DedicatedServerCallbacks,
	public network::NetworkDispatcher,
	public network::NetworkFlowListener
{
public:
	DedicatedServer(const uint8_t privateKey[],
		const yojimbo::Address& serverAddress,
		DedicatedServerAdapter& dedicatedServerAdapter);
	virtual ~DedicatedServer();

	void Run();
	bool RunFrame(double dt);

protected:
	virtual void Update(double dt) { (void)dt; }
	virtual void Render() {}

	void OnClientConnected(int clientIndex) override;
	void OnClientDisconnected(int clientIndex) override { (void)clientIndex; }
	
	void OnNetworkEvent(network::NetworkEventSPtr networkEvent) override;
	void OnReplicaObjectCreated(std::uint32_t objectType, network::ReplicaObjectSPtr replicaObject) override;
	void OnReplicaObjectDestroyed(network::NetworkObjectIdValue objectIdValue) override;

	void DispatchNetworkMessage(network::NetworkMessageSPtr networkMessage) final;
	void DispatchNetworkMessageToClient(int clientIndex, network::NetworkMessageSPtr networkMessage);
	void DispatchNetworkMessageFiltered(network::NetworkMessageSPtr networkMessage, int filteredClientIndex = -1);

private:
	void RunInternal(double dt);
	void ProcessMessages();
	void ProcessMessage(int clientIndex, yojimbo::Message* message);
	void OnCommunicationMessageReceived(int clientIndex, CommunicationMessage* communicationMessage);
};

#define GetClientIndex(networkId) networkId - 1

END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_COMMUNICATION_DEDICATED_SERVER