#ifndef HERMES_NETWORK_MASTER_STATE_CONTROLLER
#define HERMES_NETWORK_MASTER_STATE_CONTROLLER

#include "hermes/network/network_base.h"
#include "hermes/network/network_object_state_controller.h"
#include "hermes/network/network_object_state_data.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class MasterObject;

class MasterStateController : public NetworkObjectStateController
{
public:
	void SetMasterObject(MasterObject* masterObject);
	MasterObject* GetMasterObject() const;

	bool IsStateInSync(NetworkObjectStateDataSPtr stateData) const;

	NetworkObjectStateIndex GetCurrentStateIndex() const;
	void AdvanceCurrentStateIndex();

protected:
	MasterStateController(NetworkObjectStateId networkObjectStateId, float syncThreshold);
	virtual float ComputeSyncRate(NetworkObjectStateDataSPtr stateData) const = 0;

private:
	MasterObject* m_masterObject;
	NetworkObjectStateIndex m_currentStateIndex;
	float m_syncThreshold;
};

using MasterStateControllerUPtr = std::unique_ptr<MasterStateController>;
using MasterStateControllerSPtr = std::shared_ptr<MasterStateController>;

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_MASTER_STATE_CONTROLLER
