#ifndef HERMES_NETWORK_DISPATCHER
#define HERMES_NETWORK_DISPATCHER

#include "hermes/network/network_base.h"
#include "hermes/network/network_message.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

class NetworkDispatcher
{
public:
	virtual void DispatchNetworkMessage(NetworkMessageSPtr networkMessage) = 0;
	virtual ~NetworkDispatcher();
};

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_DISPATCHER
