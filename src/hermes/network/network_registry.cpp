#include "hermes/network/network_registry.h"
#include "hermes/network/network_worker.h"
#include "hermes/network/network_message.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

void NetworkRegistry::UpdateMasterObjects()
{
	for (auto it = m_masterObjects.begin(); it != m_masterObjects.end(); ++it)
	{
		MasterObjectSPtr masterObject = it->second;
		masterObject->UpdateNetwork();
	}
}

void NetworkRegistry::UpdateReplicaObjects()
{
	for (auto it = m_replicaObjects.begin(); it != m_replicaObjects.end(); ++it)
	{
		ReplicaObjectSPtr replicaObject = it->second;
		replicaObject->UpdateNetwork();
	}
}

void NetworkRegistry::PushObjectStateUpdate(const NetworkObjectStateUpdate& objectStateUpdate)
{
	NetworkObjectStateDataSPtr objectStateData = objectStateUpdate.m_objectStateData;

	if (objectStateData)
	{
		NetworkObjectStateSPtr objectState = std::make_shared<NetworkObjectState>();

		objectState->SetNetworkObjectIdValue(objectStateUpdate.m_objectIdValue);
		objectState->SetNetworkObjectStateId(objectStateUpdate.m_objectStateId);
		objectState->SetNetworkObjectStateIndex(objectStateUpdate.m_objectStateIndex);

		BEGIN_NETWORK_MESSAGE_WRITING(objectState, stateStream)
			objectStateData->SerializeInternal(stateStream);
		END_NETWORK_MESSAGE_WRITING(objectState, stateStream)

		auto networkMessage = std::static_pointer_cast<NetworkMessage>(objectState);
		GetNetworkWorker().PushNetworkMessage(networkMessage);

		OnObjectStateReceived(objectState);
	}
}

void NetworkRegistry::OnObjectStateReceived(NetworkObjectStateSPtr objectState)
{
	if (!objectState)
		return;

	NetworkObjectIdValue objectIdValue = objectState->GetNetworkObjectIdValue();

	auto it = m_replicaObjects.find(objectIdValue);

	if (it != m_replicaObjects.end())
	{
		ReplicaObjectSPtr replicaObject = it->second;
		replicaObject->OnObjectStateReceived(objectState);
	}
}

bool NetworkRegistry::RegisterMasterObject(MasterObjectSPtr masterObject)
{
	if (!masterObject)
		return false;

	NetworkObjectIdValue objectIdValue = masterObject->GetNetworkObjectId().GetValue();

	auto it = m_masterObjects.find(objectIdValue);

	if (it == m_masterObjects.end())
	{
		ReplicaObjectSPtr replicaObject = masterObject->GetReplicaObject();

		if (RegisterReplicaObject(replicaObject))
		{
			m_masterObjects.emplace(objectIdValue, masterObject);
			return true;
		}
	}

	return false;
}

bool NetworkRegistry::UnregisterMasterObject(NetworkObjectIdValue objectIdValue)
{
	auto it = m_masterObjects.find(objectIdValue);

	if (it != m_masterObjects.end())
	{
		UnregisterReplicaObject(objectIdValue);
		m_masterObjects.erase(it);
		return true;
	}

	return false;
}

bool NetworkRegistry::RegisterReplicaObject(ReplicaObjectSPtr replicaObject)
{
	if (!replicaObject)
		return false;

	NetworkObjectIdValue objectIdValue = replicaObject->GetNetworkObjectId().GetValue();

	auto it = m_replicaObjects.find(objectIdValue);

	if (it == m_replicaObjects.end())
	{
		m_replicaObjects.emplace(objectIdValue, replicaObject);
		return true;
	}

	return false;
}

bool NetworkRegistry::UnregisterReplicaObject(NetworkObjectIdValue objectIdValue)
{
	auto it = m_replicaObjects.find(objectIdValue);

	if (it != m_replicaObjects.end())
	{
		m_replicaObjects.erase(it);
		return true;
	}

	return false;
}

bool NetworkRegistry::IsMasterObjectRegistered(NetworkObjectIdValue objectIdValue)
{
	return m_masterObjects.find(objectIdValue) != m_masterObjects.end();
}

bool NetworkRegistry::IsReplicaObjectRegistered(NetworkObjectIdValue objectIdValue)
{
	return m_replicaObjects.find(objectIdValue) != m_replicaObjects.end();
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE