#ifndef HERMES_NETWORK_REPLICA_STATE_CONTROLLER
#define HERMES_NETWORK_REPLICA_STATE_CONTROLLER

#include "hermes/network/network_base.h"
#include "hermes/network/network_object_state_controller.h"
#include "hermes/network/network_object_state_data.h"
#include "hermes/network/network_object_state.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class ReplicaObject;

class ReplicaStateController : public NetworkObjectStateController
{
public:
	void SetReplicaObject(ReplicaObject* replicaObject);
	ReplicaObject* GetReplicaObject() const;

	virtual void UpdateReplicaState() const = 0;
	void OnObjectStateReceived(NetworkObjectStateSPtr objectState);

protected:
	ReplicaStateController(NetworkObjectStateId networkObjectStateId, std::uint16_t maxNumOfStateData);
	virtual NetworkObjectStateDataSPtr ParseStateData(NetworkObjectStateSPtr objectState) const = 0;

	const std::deque<NetworkObjectStateDataSPtr>& GetLastStateData() const;
	void OnStateDataReceived(NetworkObjectStateDataSPtr objectStateData);

private:
	ReplicaObject* m_replicaObject;
	NetworkObjectStateIndex m_lastObjectStateIndex;
	std::deque<NetworkObjectStateDataSPtr> m_lastStateData;
	std::uint16_t m_maxNumOfStateData;
};

using ReplicaStateControllerUPtr = std::unique_ptr<ReplicaStateController>;
using ReplicaStateControllerSPtr = std::shared_ptr<ReplicaStateController>;

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_REPLICA_STATE_CONTROLLER
