#ifndef HERMES_NETWORK_FLOW
#define HERMES_NETWORK_FLOW

#include "hermes/network/network_base.h"
#include "hermes/network/master_object.h"
#include "hermes/network/network_object_init.h"
#include "hermes/network/network_event_type.h"
#include "hermes/network/network_event_data.h"
#include "hermes/network/network_event.h"
#include "hermes/system/singleton.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class NetworkFlowListener;

class NetworkFlow final : public system::Singleton<NetworkFlow>
{
	SINGLETON_ACCESS(NetworkFlow)

public:
	bool IsInitialized() const;
	void Initialize(NetworkId networkId);
	NetworkId GetNetworkId() const;

	bool RegisterNetworkFlowListener(NetworkFlowListener* networkFlowListener);
	bool UnregisterNetworkFlowListener(NetworkFlowListener* networkFlowListener);

	void OnEventReceived(NetworkEventSPtr networkEvent);
	void PushNetworkEvent(NetworkEventType eventType, NetworkEventData& eventData);

	MasterObjectSPtr CreateMasterObject(std::uint32_t objectType, NetworkObjectInitDataSPtr objectInitData = nullptr);
	void DestroyMasterObject(NetworkObjectIdValue objectIdValue);

private:
	void OnNetworkFlowInitializeEvent(NetworkFlowInitializeEventData& eventData);
	void OnCreateReplicaObjectEvent(CreateReplicaObjectEventData& eventData);
	void OnDestroyReplicaObjectEvent(DestroyReplicaObjectEventData& eventData);

	NetworkId m_networkId;
	NetworkFlowListener* m_networkFlowListener;
	ObjectId m_currentObjectId;

	NetworkFlow();
	~NetworkFlow() override {}
};

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#define GetNetworkFlow() hermes::network::NetworkFlow::GetInstance()

#endif // HERMES_NETWORK_FLOW