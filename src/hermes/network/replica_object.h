#ifndef HERMES_NETWORK_REPLICA_OBJECT
#define HERMES_NETWORK_REPLICA_OBJECT

#include "hermes/network/network_base.h"
#include "hermes/network/network_object.h"
#include "hermes/network/network_object_id.h"
#include "hermes/network/network_object_type.h"
#include "hermes/network/network_object_state_data.h"
#include "hermes/network/network_object_state.h"
#include "hermes/network/replica_state_controller.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class ReplicaObject : public NetworkObject
{
public:
	NetworkObjectType GetNetworkObjectType() const final;
	void UpdateNetwork() final;

	template <class ReplicaStateControllerType>
	bool RegisterStateController(NetworkObjectStateId objectStateId, std::uint16_t maxNumOfStateData = 10);
	bool UnregisterStateController(NetworkObjectStateId objectStateId);

	void OnObjectStateReceived(NetworkObjectStateSPtr objectState);
	NetworkObjectStateDataSPtr GetNetworkObjectStateData(NetworkObjectStateId objectStateId) const final;

protected:
	ReplicaObject(NetworkObjectId networkObjectId);

private:
	std::unordered_map<NetworkObjectStateId, ReplicaStateControllerUPtr> m_stateControllers;
};

template <class ReplicaStateControllerType> bool ReplicaObject::RegisterStateController(NetworkObjectStateId objectStateId, std::uint16_t maxNumOfStateData)
{
	auto it = m_stateControllers.find(objectStateId);

	if (it == m_stateControllers.end())
	{
		ReplicaStateController* replicaStateController = new ReplicaStateControllerType(objectStateId, maxNumOfStateData);
		replicaStateController->SetReplicaObject(this);

		m_stateControllers.emplace(objectStateId, ReplicaStateControllerUPtr(replicaStateController));
		return true;
	}

	return false;
}

using ReplicaObjectUPtr = std::unique_ptr<ReplicaObject>;
using ReplicaObjectSPtr = std::shared_ptr<ReplicaObject>;

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_REPLICA_OBJECT
