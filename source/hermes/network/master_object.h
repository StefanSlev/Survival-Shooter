#ifndef HERMES_NETWORK_MASTER_OBJECT
#define HERMES_NETWORK_MASTER_OBJECT

#include "hermes/network/network_base.h"
#include "hermes/network/network_object.h"
#include "hermes/network/network_object_id.h"
#include "hermes/network/network_object_type.h"
#include "hermes/network/network_object_state_data.h"
#include "hermes/network/master_state_controller.h"
#include "hermes/network/replica_object.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class MasterObject : public NetworkObject
{
public:
	NetworkObjectType GetNetworkObjectType() const final;

	void SetReplicaObject(ReplicaObjectSPtr replicaObject);
	ReplicaObjectSPtr GetReplicaObject() const;

	void UpdateNetwork() final;

	template <class MasterStateControllerType>
	bool RegisterStateController(NetworkObjectStateId objectStateId, float syncThreshold = 1.0f);
	bool UnregisterStateController(NetworkObjectStateId objectStateId);

	NetworkObjectStateDataSPtr GetNetworkObjectStateData(NetworkObjectStateId objectStateId) const final;

protected:
	MasterObject(NetworkObjectId networkObjectId);

private:
	ReplicaObjectSPtr m_replicaObject;
	std::unordered_map<NetworkObjectStateId, MasterStateControllerUPtr> m_stateControllers;
};

template <class MasterStateControllerType> bool MasterObject::RegisterStateController(NetworkObjectStateId objectStateId, float syncThreshold)
{
	auto it = m_stateControllers.find(objectStateId);

	if (it == m_stateControllers.end())
	{
		MasterStateController* masterStateController = new MasterStateControllerType(objectStateId, syncThreshold);
		masterStateController->SetMasterObject(this);

		m_stateControllers.emplace(objectStateId, MasterStateControllerUPtr(masterStateController));
		return true;
	}

	return false;
}

using MasterObjectUPtr = std::unique_ptr<MasterObject>;
using MasterObjectSPtr = std::shared_ptr<MasterObject>;

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_MASTER_OBJECT
