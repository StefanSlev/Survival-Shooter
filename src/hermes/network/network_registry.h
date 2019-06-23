#ifndef HERMES_NETWORK_REGISTRY
#define HERMES_NETWORK_REGISTRY

#include "hermes/network/network_base.h"
#include "hermes/network/master_object.h"
#include "hermes/network/replica_object.h"
#include "hermes/network/network_object_id.h"
#include "hermes/network/network_object_type.h"
#include "hermes/network/network_object_state.h"
#include "hermes/network/network_object_state_data.h"
#include "hermes/system/singleton.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class NetworkRegistry final : public system::Singleton<NetworkRegistry>
{
	SINGLETON_ACCESS(NetworkRegistry)

public:
	struct NetworkObjectStateUpdate
	{
		NetworkObjectIdValue m_objectIdValue;
		NetworkObjectStateId m_objectStateId;
		NetworkObjectStateIndex m_objectStateIndex;
		NetworkObjectStateDataSPtr m_objectStateData;
	};

	void UpdateMasterObjects();
	void UpdateReplicaObjects();

	void OnObjectStateReceived(NetworkObjectStateSPtr objectState);
	void PushObjectStateUpdate(const NetworkObjectStateUpdate& objectStateUpdate);

	bool RegisterMasterObject(MasterObjectSPtr masterObject);
	bool UnregisterMasterObject(NetworkObjectIdValue objectIdValue);

	bool RegisterReplicaObject(ReplicaObjectSPtr replicaObject);
	bool UnregisterReplicaObject(NetworkObjectIdValue objectIdValue);

	bool IsMasterObjectRegistered(NetworkObjectIdValue objectIdValue);
	bool IsReplicaObjectRegistered(NetworkObjectIdValue objectIdValue);

private:
	std::unordered_map<NetworkObjectIdValue, MasterObjectSPtr> m_masterObjects;
	std::unordered_map<NetworkObjectIdValue, ReplicaObjectSPtr> m_replicaObjects;

	NetworkRegistry() {}
	~NetworkRegistry() override {}
};

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#define GetNetworkRegistry() hermes::network::NetworkRegistry::GetInstance()

#endif // HERMES_NETWORK_REGISTRY