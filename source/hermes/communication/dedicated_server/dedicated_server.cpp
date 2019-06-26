#include "hermes/communication/dedicated_server/dedicated_server.h"
#include "hermes/communication/communication_config.h"
#include "hermes/communication/communication_message.h"
// Online
#include "hermes/online/online_worker.h"
#include "hermes/online/online_registry.h"
// Network
#include "hermes/network/network_flow.h"
#include "hermes/network/network_registry.h"
#include "hermes/network/network_worker.h"
#include "hermes/network/network_object_type.h"
#include "hermes/network/network_message_type.h"
#include "hermes/network/network_channel_type.h"
#include "hermes/network/network_object_state.h"
#include "hermes/network/network_event.h"
#include "hermes/network/network_event_type.h"
#include "hermes/network/network_event_data.h"

BEGIN_HERMES_NAMESPACE

BEGIN_COMMUNICATION_NAMESPACE

DedicatedServer::DedicatedServer(const uint8_t privateKey[],
	const yojimbo::Address& serverAddress,
	DedicatedServerAdapter& dedicatedServerAdapter) :
	Server(yojimbo::GetDefaultAllocator(),
		privateKey,
		serverAddress,
		CommunicationConfig(),
		dedicatedServerAdapter,
		0)
{
	GetNetworkFlow().Initialize(0);
	GetNetworkFlow().RegisterNetworkFlowListener(this);
	GetNetworkWorker().RegisterNetworkDispatcher(this);
}

DedicatedServer::~DedicatedServer()
{
	GetNetworkFlow().UnregisterNetworkFlowListener(this);
	GetNetworkWorker().UnregisterNetworkDispatcher(this);
}

void DedicatedServer::Run()
{
	float fixedDt = 1.0f / 60.0f;

	while (IsRunning())
	{
		double currentTime = yojimbo_time();
		double serverTime = GetTime();

		if (serverTime <= currentTime)
		{
			RunInternal(fixedDt);
			AdvanceTime(serverTime + fixedDt);
		}
		else
		{
			yojimbo_sleep(serverTime - currentTime);
		}
	}
}

bool DedicatedServer::RunFrame(double dt)
{
	if (IsRunning())
	{
		RunInternal(dt);
		AdvanceTime(GetTime() + dt);

		Render();
		return true;
	}

	return false;
}

void DedicatedServer::RunInternal(double dt)
{
	// receive and process messages from network
	ReceivePackets();

	ProcessMessages();
	GetNetworkRegistry().UpdateReplicaObjects();

	// update online module
	GetOnlineWorker().Update();
	GetOnlineRegistry().Update();

	// update server
	Update(dt);

	// update network module
	 GetNetworkRegistry().UpdateMasterObjects();

	// send packets on network
	SendPackets();
}

void DedicatedServer::ProcessMessages()
{
	int numConnectedClients = GetNumConnectedClients();

	for (int clientIndex = 0; clientIndex < numConnectedClients; ++clientIndex)
	{
		if (IsClientConnected(clientIndex))
		{
			for (CommunicationChannelType communicationChannelType : communicationChannelTypes)
			{
				int channelIndex = GetCommunicationChannelIndex(communicationChannelType);
				yojimbo::Message* message = ReceiveMessage(clientIndex, channelIndex);

				while (message != nullptr)
				{
					ProcessMessage(clientIndex, message);
					ReleaseMessage(clientIndex, message);
					message = ReceiveMessage(clientIndex, channelIndex);
				}
			}
		}
	}
}

void DedicatedServer::ProcessMessage(int clientIndex, yojimbo::Message* message)
{
	if (!message || !IsClientConnected(clientIndex))
		return;

	switch (message->GetType())
	{
	case GetCommunicationMessageIndex(CommunicationMessageType::CommunicationMessage):
	{
		CommunicationMessage* communicationMessage = static_cast<CommunicationMessage*> (message);
		OnCommunicationMessageReceived(clientIndex, communicationMessage);
		break;
	}
	default:
		break;
	}
}

void DedicatedServer::OnCommunicationMessageReceived(int clientIndex, CommunicationMessage* communicationMessage)
{
	if (!communicationMessage || !IsClientConnected(clientIndex))
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

		DispatchNetworkMessageFiltered(networkMessage, clientIndex);
		GetNetworkWorker().OnNetworkMessageReceived(networkMessage);
	}
}

void DedicatedServer::DispatchNetworkMessage(network::NetworkMessageSPtr networkMessage)
{
	if (!networkMessage)
		return;

	DispatchNetworkMessageFiltered(networkMessage);
}

void DedicatedServer::DispatchNetworkMessageFiltered(network::NetworkMessageSPtr networkMessage, int filteredClientIndex)
{
	if (!networkMessage)
		return;

	int numConnectedClients = GetNumConnectedClients();

	for (int clientIndex = 0; clientIndex < numConnectedClients; ++clientIndex)
	{
		if (IsClientConnected(clientIndex) && clientIndex != filteredClientIndex)
		{
			DispatchNetworkMessageToClient(clientIndex, networkMessage);
		}
	}
}

void DedicatedServer::DispatchNetworkMessageToClient(int clientIndex, network::NetworkMessageSPtr networkMessage)
{
	if (!networkMessage || !IsClientConnected(clientIndex))
		return;

	network::NetworkMessageType networkMessageType = networkMessage->GetNetworkMessageType();
	network::NetworkChannelType networkChannelType = networkMessage->GetNetworkChannelType();

	CommunicationChannelType channelType = CommunicationChannelType::Unreliable;

	if (networkChannelType == network::NetworkChannelType::ReliableOrdered)
	{
		channelType = CommunicationChannelType::Reliable;
	}

	yojimbo::Message* message = CreateMessage(clientIndex, GetCommunicationMessageIndex(CommunicationMessageType::CommunicationMessage));
	
	if (message)
	{
		CommunicationMessage* communicationMessage = static_cast<CommunicationMessage*> (message);

		communicationMessage->SetTimestamp(GetTime());
		communicationMessage->SetMessageType(GetNetworkMessageIndex(networkMessageType));

		BEGIN_COMMUNICATION_MESSAGE_WRITING(communicationMessage, communicationStream)
			networkMessage->SerializeInternal(communicationStream);
		END_COMMUNICATION_MESSAGE_WRITING(communicationMessage, communicationStream)

		int channelIndex = GetCommunicationChannelIndex(channelType);
		
		if (CanSendMessage(clientIndex, channelIndex))
			SendMessage(clientIndex, channelIndex, communicationMessage);
	}
}

void DedicatedServer::OnClientConnected(int clientIndex)
{
	if (!IsClientConnected(clientIndex))
		return;

	network::NetworkFlowInitializeEventData eventData;
	eventData.SetNetworkId(clientIndex + 1);
	network::NetworkEventType eventType = network::NetworkEventType::NetworkFlowInitialize;

	auto networkEvent = std::make_shared<network::NetworkEvent>();
	networkEvent->SetNetworkEventType(GetNetworkEventTypeIndex(eventType));

	BEGIN_NETWORK_MESSAGE_WRITING(networkEvent, eventStream)
		eventData.SerializeInternal(eventStream);
	END_NETWORK_MESSAGE_WRITING(networkEvent, eventStream)

	auto networkMessage = std::static_pointer_cast<network::NetworkMessage>(networkEvent);
	DispatchNetworkMessageToClient(clientIndex, networkMessage);
}

void DedicatedServer::OnNetworkEvent(network::NetworkEventSPtr networkEvent)
{
	(void)networkEvent;
}

void DedicatedServer::OnReplicaObjectCreated(std::uint32_t objectType, network::ReplicaObjectSPtr replicaObject)
{
	(void)objectType;
	(void)replicaObject;
}

void DedicatedServer::OnReplicaObjectDestroyed(network::NetworkObjectIdValue objectIdValue)
{
	(void)objectIdValue;
}

END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE