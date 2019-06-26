#ifndef HERMES_COMMUNICATION_DEDICATED_SERVER_CALLBACKS
#define HERMES_COMMUNICATION_DEDICATED_SERVER_CALLBACKS

#include "hermes/communication/communication_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_COMMUNICATION_NAMESPACE

class DedicatedServerCallbacks
{
public:
	virtual void OnClientConnected(int clientIndex) = 0;
	virtual void OnClientDisconnected(int clientIndex) = 0;
};

END_COMMUNICATION_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_COMMUNICATION_DEDICATED_SERVER_CALLBACKS