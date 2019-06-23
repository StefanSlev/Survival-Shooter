#include "hermes/network/master_state_controller.h"
#include "hermes/network/master_object.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

MasterStateController::MasterStateController(NetworkObjectStateId networkObjectStateId, float syncThreshold)
	: NetworkObjectStateController(networkObjectStateId)
{
	syncThreshold = fmaxf(syncThreshold, 0.f);
	this->m_syncThreshold = fminf(syncThreshold, 1.f);
	this->m_currentStateIndex = 0;
}

bool MasterStateController::IsStateInSync(NetworkObjectStateDataSPtr stateData) const
{
	return (ComputeSyncRate(stateData) - m_syncThreshold > -1e-3);
}

NetworkObjectStateIndex MasterStateController::GetCurrentStateIndex() const
{
	return this->m_currentStateIndex;
}

void MasterStateController::AdvanceCurrentStateIndex()
{
	this->m_currentStateIndex++;
}

void MasterStateController::SetMasterObject(MasterObject* masterObject)
{
	this->m_masterObject = masterObject;
}

MasterObject* MasterStateController::GetMasterObject() const
{
	return this->m_masterObject;
}

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE