#include "hermes/network/replica_object.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

ReplicaObject::ReplicaObject(NetworkObjectId networkObjectId) : NetworkObject(networkObjectId)
{
}

NetworkObjectType ReplicaObject::GetNetworkObjectType() const
{
	return NetworkObjectType::Replica;
}

void ReplicaObject::UpdateNetwork()
{
	for (auto it = m_stateControllers.begin(); it != m_stateControllers.end(); ++it)
	{
		ReplicaStateControllerUPtr& replicaStateController = it->second;
		replicaStateController->UpdateReplicaState();
	}
}

void ReplicaObject::OnObjectStateReceived(NetworkObjectStateSPtr objectState)
{
	if (!objectState)
		return;

	NetworkObjectIdValue objectIdValue = objectState->GetNetworkObjectIdValue();

	if (GetNetworkObjectId().GetValue() != objectIdValue)
		return;

	NetworkObjectStateId objectStateId = objectState->GetNetworkObjectStateId();

	auto it = m_stateControllers.find(objectStateId);

	if (it != m_stateControllers.end())
	{
		ReplicaStateControllerUPtr& replicaStateController = it->second;
		replicaStateController->OnObjectStateReceived(objectState);
	}
}

NetworkObjectStateDataSPtr ReplicaObject::GetNetworkObjectStateData(NetworkObjectStateId objectStateId) const
{
	auto it = m_stateControllers.find(objectStateId);

	if (it != m_stateControllers.cend())
	{
		const ReplicaStateControllerUPtr& replicaStateController = it->second;
		return replicaStateController->GetCurrentStateData();
	}

	return nullptr;
}

bool ReplicaObject::UnregisterStateController(NetworkObjectStateId objectStateId)
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