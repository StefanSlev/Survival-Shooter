#ifndef HERMES_COMMUNICATION_GAME_CLIENT
#define HERMES_COMMUNICATION_GAME_CLIENT

#include "hermes/communication/game_client/game_client_state.h"
#include "hermes/communication/communication_base.h"
#include "hermes/communication/communication_adapter.h"
#include "hermes/network/network_message.h"
#include "hermes/network/network_dispatcher.h"
#include "hermes/network/network_flow_listener.h"
#include "hermes/network/network_event.h"
#include "hermes/network/replica_object.h"
#include "hermes/network/network_object_id.h"

BEGIN_HERMES_NAMESPACE

BEGIN_COMMUNICATION_NAMESPACE

class CommunicationMessage;

class GameClient : 
	public yojimbo::Client, 
	public network::NetworkDispatcher,
	public network::NetworkFlowListener
{
public:
	GameClient(const yojimbo::Address& address, 
			   CommunicationAdapter& communicationAdapter);
	virtual ~GameClient();

	bool RunFrame(double dt);

protected:
	virtual void Update(double dt) { (void) dt; }
	virtual void Render() {}
	
	void OnNetworkEvent(network::NetworkEventSPtr networkEvent) override;
	void OnReplicaObjectCreated(std::uint32_t objectType, network::ReplicaObjectSPtr replicaObject) override;
	void OnReplicaObjectDestroyed(network::NetworkObjectIdValue objectIdValue) override;

	GameClientState GetGameClientState();
	bool IsGameClientState(GameClientState gameClientState);

private:
	void UpdateNetwork();
	void ProcessMessages();
	void ProcessMessage(yojimbo::Message* message);
	void OnCommunicationMessageReceived(CommunicationMessage* communicationMessage);
	void DispatchNetworkMessage(network::NetworkMessageSPtr networkMessage) final;
	void GoToGameClientState(GameClientState gameClientState);

	GameClientState m_gameClientState;
};

#define DISCONNECTED_BARRIER() \
    do                         \
    {                          \
        if (IsDisconnected())  \
            return false;      \
    }                          \
    while(0);

#define CONNECTION_FAILURE_BARRIER() \
    do                               \
    {                                \
        if (ConnectionFailed())      \
            return false;            \
    }                                \
    while(0);


END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_COMMUNICATION_GAME_CLIENT