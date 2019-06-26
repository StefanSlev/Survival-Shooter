#include "hermes/communication/game_client/game_client.h"
#include "hermes/communication/communication_config.h"
#include "hermes/communication/communication_message.h"
// Online
#include "hermes/online/online_worker.h"
#include "hermes/online/online_registry.h"
// Network
#include "hermes/network/network_worker.h"
#include "hermes/network/network_flow.h"
#include "hermes/network/network_registry.h"
#include "hermes/network/network_object_type.h"
#include "hermes/network/network_message_type.h"
#include "hermes/network/network_channel_type.h"
#include "hermes/network/network_object_state.h"
#include "hermes/network/network_event.h"

BEGIN_HERMES_NAMESPACE

BEGIN_COMMUNICATION_NAMESPACE

GameClient::GameClient(const yojimbo::Address& address, 
					   CommunicationAdapter& communicationAdapter) : 
	Client(yojimbo::GetDefaultAllocator(),
		   address,
		   CommunicationConfig(),
		   communicationAdapter,
		   0)
{
	GetNetworkFlow().RegisterNetworkFlowListener(this);
	GetNetworkWorker().RegisterNetworkDispatcher(this);
	GoToGameClientState(GameClientState::WaitingForNetwork);
}

GameClient::~GameClient()
{
	GetNetworkFlow().UnregisterNetworkFlowListener(this);
	GetNetworkWorker().UnregisterNetworkDispatcher(this);
}

bool GameClient::RunFrame(double dt)
{
	// receive and process messages from network
	ReceivePackets();
	DISCONNECTED_BARRIER()
	ProcessMessages();

	// update network
	GetNetworkRegistry().UpdateReplicaObjects();
	UpdateNetwork();

	// update online module
	GetOnlineWorker().Update();
	GetOnlineRegistry().Update();
	
	Update(dt);

	if (IsGameClientState(GameClientState::NetworkReady))
		GetNetworkRegistry().UpdateMasterObjects();

	// send packets on network
	SendPackets();
	DISCONNECTED_BARRIER()
	AdvanceTime(GetTime() + dt);
	CONNECTION_FAILURE_BARRIER()

	Render();
	return true;
}

void GameClient::UpdateNetwork()
{
	switch (GetGameClientState())
	{
		case GameClientState::WaitingForNetwork:
		{
			if (GetNetworkFlow().IsInitialized())
			{
				GoToGameClientState(GameClientState::NetworkReady);
			}
			break;
		}
		case GameClientState::NetworkReady:
		{
			break;
		}
		default:
			break;
	}
}

void GameClient::ProcessMessages()
{
	if (!IsConnected())
		return;

	for (CommunicationChannelType communicationChannelType : communicationChannelTypes)
	{
		int channelIndex = GetCommunicationChannelIndex(communicationChannelType);
		yojimbo::Message* message = ReceiveMessage(channelIndex);

		while (message != nullptr)
		{
			ProcessMessage(message);
			ReleaseMessage(message);
			message = ReceiveMessage(channelIndex);
		}
	}
}

void GameClient::ProcessMessage(yojimbo::Message* message)
{
	if (!message || !IsConnected())
		return;

	switch (message->GetType())
	{
	case GetCommunicationMessageIndex(CommunicationMessageType::CommunicationMessage):
	{
		CommunicationMessage* communicationMessage = static_cast<CommunicationMessage*> (message);
		OnCommunicationMessageReceived(communicationMessage);
		break;
	}
	default:
		break;
	}
}

void GameClient::OnCommunicationMessageReceived(CommunicationMessage* communicationMessage)
{
	if (!communicationMessage || !IsConnected())
		return;

	network::NetworkMessageSPtr networkMessage = nullptr;

	switch (communicationMessage->GetMessageType())
	{
	case GetNetworkMessageIndex(network::NetworkMessageType::NetworkObjectState):
	{
		auto networkObjectState = std::make_shared<network::NetworkObjectState>();
		networkMessage = std::static_pointer_cast<network::NetworkMessage>(networkObjectState);
		break;
	}
	case GetNetworkMessageIndex(network::NetworkMessageType::NetworkEvent):
	{
		auto networkEvent = std::make_shared<network::NetworkEvent>();
		networkMessage = std::static_pointer_cast<network::NetworkMessage>(networkEvent);
		break;
	}
	default:
		break;
	}

	if (networkMessage)
	{
		BEGIN_COMMUNICATION_MESSAGE_READING(communicationMessage, communicationStream)
			networkMessage->SerializeInternal(communicationStream);
		END_COMMUNICATION_MESSAGE_READING(communicationMessage, communicationStream)

		GetNetworkWorker().OnNetworkMessageReceived(networkMessage);
	}
}

void GameClient::DispatchNetworkMessage(network::NetworkMessageSPtr networkMessage)
{
	if (!networkMessage || !IsConnected())
		return;

	network::NetworkMessageType networkMessageType = networkMessage->GetNetworkMessageType();
	network::NetworkChannelType networkChannelType = networkMessage->GetNetworkChannelType();

	CommunicationChannelType channelType = CommunicationChannelType::Unreliable;

	if (networkChannelType == network::NetworkChannelType::ReliableOrdered)
	{
		channelType = CommunicationChannelType::Reliable;
	}

	yojimbo::Message* message = CreateMessage(GetCommunicationMessageIndex(CommunicationMessageType::CommunicationMessage));
	
	if (message)
	{
		CommunicationMessage* communicationMessage = static_cast<CommunicationMessage*> (message);

		communicationMessage->SetTimestamp(this->GetTime());
		communicationMessage->SetMessageType(GetNetworkMessageIndex(networkMessageType));

		BEGIN_COMMUNICATION_MESSAGE_WRITING(communicationMessage, communicationStream)
			networkMessage->SerializeInternal(communicationStream);
		END_COMMUNICATION_MESSAGE_WRITING(communicationMessage, communicationStream)

		int channelIndex = GetCommunicationChannelIndex(channelType);

		if (CanSendMessage(channelIndex))
			SendMessage(channelIndex, communicationMessage);
	}
}

void GameClient::OnNetworkEvent(network::NetworkEventSPtr networkEvent)
{
	(void)networkEvent;
}

void GameClient::OnReplicaObjectCreated(std::uint32_t objectType, network::ReplicaObjectSPtr replicaObject)
{
	(void)objectType;
	(void)replicaObject;
}

void GameClient::OnReplicaObjectDestroyed(network::NetworkObjectIdValue objectIdValue)
{
	(void)objectIdValue;
}

GameClientState GameClient::GetGameClientState()
{
	return this->m_gameClientState;
}

void GameClient::GoToGameClientState(GameClientState gameClientState)
{
	this->m_gameClientState = gameClientState;
}

bool GameClient::IsGameClientState(GameClientState gameClientState)
{
	return m_gameClientState == gameClientState;
}

END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE