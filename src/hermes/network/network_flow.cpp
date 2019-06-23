#include "hermes/network/network_flow.h"
#include "hermes/network/network_flow_listener.h"
#include "hermes/network/network_object_id.h"
#include "hermes/network/network_object_factory.h"
#include "hermes/network/replica_object.h"
#include "hermes/network/network_registry.h"
#include "hermes/network/network_worker.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

NetworkFlow::NetworkFlow()
{
	this->m_networkId = InvalidNetworkId;
	this->m_networkFlowListener = nullptr;
	this->m_currentObjectId = 0;
}

bool NetworkFlow::IsInitialized() const
{
	return m_networkId != InvalidNetworkId;
}

void NetworkFlow::Initialize(NetworkId networkId)
{
	this->m_networkId = networkId;
}

NetworkId NetworkFlow::GetNetworkId() const
{
	return this->m_networkId;
}

bool NetworkFlow::RegisterNetworkFlowListener(NetworkFlowListener* networkFlowListener)
{
	if (!networkFlowListener)
		return false;

	if (m_networkFlowListener == nullptr)
	{
		m_networkFlowListener = networkFlowListener;
		return true;
	}

	return false;
}

bool NetworkFlow::UnregisterNetworkFlowListener(NetworkFlowListener* networkFlowListener)
{
	if (!networkFlowListener)
		return false;

	if (m_networkFlowListener == networkFlowListener)
	{
		m_networkFlowListener = nullptr;
		return true;
	}

	return false;
}

void NetworkFlow::OnEventReceived(NetworkEventSPtr networkEvent)
{
	if (!networkEvent)
		return;

	switch (networkEvent->GetNetworkEventType())
	{
	case GetNetworkEventTypeIndex(NetworkEventType::NetworkFlow):
	{
		if (m_networkFlowListener)
			m_networkFlowListener->OnNetworkEvent(networkEvent);

		break;
	}
	case GetNetworkEventTypeIndex(NetworkEventType::NetworkFlowInitialize):
	{
		NetworkFlowInitializeEventData networkFlowInitializeEventData;

		BEGIN_NETWORK_MESSAGE_READING(networkEvent, eventStream)
			networkFlowInitializeEventData.SerializeInternal(eventStream);
		END_NETWORK_MESSAGE_READING(networkEvent, eventStream)

		OnNetworkFlowInitializeEvent(networkFlowInitializeEventData);
		break;
	}
	case GetNetworkEventTypeIndex(NetworkEventType::CreateReplicaObject):
	{
		CreateReplicaObjectEventData createReplicaObjectEventData;

		BEGIN_NETWORK_MESSAGE_READING(networkEvent, eventStream)
			createReplicaObjectEventData.SerializeInternal(eventStream);
		END_NETWORK_MESSAGE_READING(networkEvent, eventStream)

		OnCreateReplicaObjectEvent(createReplicaObjectEventData);
		break;
	}
	case GetNetworkEventTypeIndex(NetworkEventType::DestroyReplicaObject):
	{
		DestroyReplicaObjectEventData destroyReplicaObjectEventData;

		BEGIN_NETWORK_MESSAGE_READING(networkEvent, eventStream)
			destroyReplicaObjectEventData.SerializeInternal(eventStream);
		END_NETWORK_MESSAGE_READING(networkEvent, eventStream)

		OnDestroyReplicaObjectEvent(destroyReplicaObjectEventData);
		break;
	}
	default:
		break;
	}
}

void NetworkFlow::PushNetworkEvent(NetworkEventType eventType, NetworkEventData& eventData)
{
	if (!IsInitialized())
		return;

	NetworkEventSPtr networkEvent = std::make_shared<NetworkEvent>();
	std::uint32_t networkEventType = GetNetworkEventTypeIndex(eventType);

	networkEvent->SetNetworkEventType(networkEventType);

	BEGIN_NETWORK_MESSAGE_WRITING(networkEvent, eventStream)
		eventData.SerializeInternal(eventStream);
	END_NETWORK_MESSAGE_WRITING(networkEvent, eventStream)

	auto networkMessage = std::static_pointer_cast<NetworkMessage>(networkEvent);
	GetNetworkWorker().PushNetworkMessage(networkMessage);
}

MasterObjectSPtr NetworkFlow::CreateMasterObject(std::uint32_t objectType, NetworkObjectInitDataSPtr objectInitData)
{
	if (!IsInitialized())
		return nullptr;

	NetworkObjectId networkObjectId;
	networkObjectId.SetValue(m_networkId, m_currentObjectId);

	NetworkObjectFactory& objectFactory = GetNetworkObjectFactory();
	MasterObjectSPtr masterObject = objectFactory.BuildMasterObject(objectType, networkObjectId);
	ReplicaObjectSPtr replicaObject = objectFactory.BuildReplicaObject(objectType, networkObjectId);

	if (masterObject && replicaObject)
	{
		masterObject->SetReplicaObject(replicaObject);

		if (GetNetworkRegistry().RegisterMasterObject(masterObject))
		{
			CreateReplicaObjectEventData createReplicaObjectEventData;
			createReplicaObjectEventData.SetObjectType(objectType);
			createReplicaObjectEventData.SetNetworkObjectIdValue(networkObjectId.GetValue());
			createReplicaObjectEventData.SetHasNetworkObjectInitData(false);

			if (objectInitData)
			{
				NetworkObjectInitWrapper& objectInitWrapper = createReplicaObjectEventData.GetNetworkObjectInitWrapper();

				auto objectInit = [&objectInitWrapper, &objectInitData](NetworkObjectSPtr networkObject)
				{
					BEGIN_OBJECT_INIT_DATA_WRITING(objectInitWrapper, initStream)
						objectInitData->SerializeInternal(initStream);
					END_OBJECT_INIT_DATA_WRITING(objectInitWrapper, initStream)

					networkObject->OnNetworkObjectInit(objectInitWrapper);
				};

				objectInit(std::static_pointer_cast<NetworkObject>(masterObject));
				objectInit(std::static_pointer_cast<NetworkObject>(replicaObject));

				BEGIN_OBJECT_INIT_DATA_WRITING(objectInitWrapper, initStream)
					objectInitData->SerializeInternal(initStream);
				END_OBJECT_INIT_DATA_WRITING(objectInitWrapper, initStream)

				createReplicaObjectEventData.SetHasNetworkObjectInitData(true);
			}

			PushNetworkEvent(NetworkEventType::CreateReplicaObject, createReplicaObjectEventData);

			++m_currentObjectId;
			return masterObject;
		}
	}

	return nullptr;
}

void NetworkFlow::DestroyMasterObject(NetworkObjectIdValue objectIdValue)
{
	if (!IsInitialized())
		return;

	if (GetNetworkRegistry().UnregisterMasterObject(objectIdValue))
	{
		DestroyReplicaObjectEventData destroyReplicaObjectEventData;
		destroyReplicaObjectEventData.SetNetworkObjectIdValue(objectIdValue);

		PushNetworkEvent(NetworkEventType::DestroyReplicaObject, destroyReplicaObjectEventData);
	}
}

void NetworkFlow::OnNetworkFlowInitializeEvent(NetworkFlowInitializeEventData& eventData)
{
	if (IsInitialized())
		return;

	this->m_networkId = eventData.GetNetworkId();
}

void NetworkFlow::OnCreateReplicaObjectEvent(CreateReplicaObjectEventData& eventData)
{
	std::uint32_t objectType = eventData.GetObjectType();

	NetworkObjectId networkObjectId;
	networkObjectId.SetValue(eventData.GetNetworkObjectIdValue());

	ReplicaObjectSPtr replicaObject = GetNetworkObjectFactory().BuildReplicaObject(objectType, networkObjectId);

	if (replicaObject && GetNetworkRegistry().RegisterReplicaObject(replicaObject))
	{
		if (eventData.HasNetworkObjectInitData())
		{
			NetworkObjectInitWrapper& objectInitWrapper = eventData.GetNetworkObjectInitWrapper();
			replicaObject->OnNetworkObjectInit(objectInitWrapper);
		}

		if (m_networkFlowListener)
			m_networkFlowListener->OnReplicaObjectCreated(objectType, replicaObject);
	}
}

void NetworkFlow::OnDestroyReplicaObjectEvent(DestroyReplicaObjectEventData& eventData)
{
	NetworkObjectIdValue objectIdValue = eventData.GetNetworkObjectIdValue();

	if (GetNetworkRegistry().UnregisterReplicaObject(objectIdValue))
	{
		if (m_networkFlowListener)
			m_networkFlowListener->OnReplicaObjectDestroyed(objectIdValue);
	}
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE