#ifndef HERMES_NETWORK_OBJECT_STATE_CONTROLLER
#define HERMES_NETWORK_OBJECT_STATE_CONTROLLER

#include "hermes/network/network_base.h"
#include "hermes/network/network_object_state_data.h"
#include "hermes/system/non_copyable.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class NetworkObjectStateController : public system::NonCopyable
{
public:
	NetworkObjectStateId GetNetworkObjectStateId() const;
	virtual NetworkObjectStateDataSPtr GetCurrentStateData() const = 0;

protected:
	NetworkObjectStateController(NetworkObjectStateId networkObjectStateId);

private:
	NetworkObjectStateId m_networkObjectStateId;
};

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_OBJECT_STATE_CONTROLLER
