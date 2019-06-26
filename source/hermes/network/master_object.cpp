#include "hermes/network/master_object.h"
#include "hermes/network/network_registry.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

MasterObject::MasterObject(NetworkObjectId networkObjectId) : NetworkObject(networkObjectId)
{
	this->m_replicaObject = nullptr;
}

NetworkObjectType MasterObject::GetNetworkObjectType() const
{
	return NetworkObjectType::Master;
}

void MasterObject::SetReplicaObject(ReplicaObjectSPtr replicaObject)
{
	this->m_replicaObject = replicaObject;
}

ReplicaObjectSPtr MasterObject::GetReplicaObject() const
{
	return m_replicaObject;
}

void MasterObject::UpdateNetwork()
{
	if (!m_replicaObject)
		return;

	NetworkRegistry& networkRegistry = GetNetworkRegistry();
	NetworkObjectIdValue objectIdValue = GetNetworkObjectId().GetValue();

	for (auto it = m_stateControllers.begin(); it != m_stateControllers.end(); ++it)
	{
		NetworkObjectStateId objectStateId = it->first;
		MasterStateControllerUPtr& masterStateController = it->second;

		NetworkObjectStateDataSPtr replicaStateData = m_replicaObject->GetNetworkObjectStateData(objectStateId);

		if (replicaStateData && !masterStateController->IsStateInSync(replicaStateData))
		{
			NetworkRegistry::NetworkObjectStateUpdate objectStateUpdate;

			objectStateUpdate.m_objectIdValue = objectIdValue;
			objectStateUpdate.m_objectStateId = objectStateId;
			objectStateUpdate.m_objectStateIndex = masterStateController->GetCurrentStateIndex();
			objectStateUpdate.m_objectStateData = masterStateController->GetCurrentStateData();

			masterStateController->AdvanceCurrentStateIndex();
			networkRegistry.PushObjectStateUpdate(objectStateUpdate);
		}
	}
}

NetworkObjectStateDataSPtr MasterObject::GetNetworkObjectStateData(NetworkObjectStateId objectStateId) const
{
	auto it = m_stateControllers.find(objectStateId);

	if (it != m_stateControllers.cend())
	{
		const MasterStateControllerUPtr& masterStateController = it->second;
		return masterStateController->GetCurrentStateData();
	}

	return nullptr;
}

bool MasterObject::UnregisterStateController(NetworkObjectStateId objectStateId)
{
	auto it = m_stateControllers.find(objectStateId);

	if (it != m_stateControllers.end())
	{
		m_stateControllers.erase(it);
		return true;
	}

	return false;
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE