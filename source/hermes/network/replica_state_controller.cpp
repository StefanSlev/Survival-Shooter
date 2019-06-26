#include "hermes/network/replica_state_controller.h"
#include "hermes/network/replica_object.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

ReplicaStateController::ReplicaStateController(NetworkObjectStateId networkObjectStateId, std::uint16_t maxNumOfStateData)
	: NetworkObjectStateController(networkObjectStateId)
{
	this->m_maxNumOfStateData = maxNumOfStateData;
	this->m_lastObjectStateIndex = 0;
}

const std::deque<NetworkObjectStateDataSPtr>& ReplicaStateController::GetLastStateData() const
{
	return this->m_lastStateData;
}

void ReplicaStateController::SetReplicaObject(ReplicaObject* replicaObject)
{
	this->m_replicaObject = replicaObject;
}

ReplicaObject* ReplicaStateController::GetReplicaObject() const
{
	return this->m_replicaObject;
}

void ReplicaStateController::OnObjectStateReceived(NetworkObjectStateSPtr objectState)
{
	if (!objectState)
		return;

	NetworkObjectStateIndex objectStateIndex = objectState->GetNetworkObjectStateIndex();

	if (m_lastStateData.size() > 0 && m_lastObjectStateIndex >= objectStateIndex)
		return;

	NetworkObjectStateDataSPtr objectStateData = this->ParseStateData(objectState);

	if (objectStateData)
	{
		m_lastObjectStateIndex = objectStateIndex;
		this->OnStateDataReceived(objectStateData);
	}
}

void ReplicaStateController::OnStateDataReceived(NetworkObjectStateDataSPtr objectStateData)
{
	if (m_lastStateData.size() == m_maxNumOfStateData)
		m_lastStateData.pop_front();

	m_lastStateData.push_back(objectStateData);
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE