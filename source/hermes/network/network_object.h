#ifndef HERMES_NETWORK_OBJECT
#define HERMES_NETWORK_OBJECT

#include "hermes/network/network_base.h"
#include "hermes/network/network_object_type.h"
#include "hermes/network/network_object_id.h"
#include "hermes/network/network_object_init.h"
#include "hermes/network/network_object_state_data.h"
#include "hermes/system/non_copyable.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class NetworkObject : public system::NonCopyable
{
public:
	const NetworkObjectId& GetNetworkObjectId() const;
	virtual NetworkObjectType GetNetworkObjectType() const = 0;

	virtual void UpdateNetwork() {}
	
	virtual NetworkObjectInitDataSPtr GetNetworkObjectInit() const { return nullptr; }
	virtual void OnNetworkObjectInit(NetworkObjectInitWrapper& objectInitWrapper) { (void) objectInitWrapper; }
	virtual NetworkObjectStateDataSPtr GetNetworkObjectStateData(NetworkObjectStateId objectStateId) const = 0;

protected:
	NetworkObject(NetworkObjectId networkObjectId);

private:
	NetworkObjectId m_networkObjectId;
};

using NetworkObjectUPtr = std::unique_ptr<NetworkObject>;
using NetworkObjectSPtr = std::shared_ptr<NetworkObject>;

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_OBJECT
