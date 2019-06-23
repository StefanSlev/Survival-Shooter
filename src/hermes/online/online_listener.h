#ifndef HERMES_ONLINE_LISTENER
#define HERMES_ONLINE_LISTENER

#include "hermes/online/online_base.h"
#include "hermes/online/online_operation.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

class OnlineListener
{
public:
	virtual void OnOperationCompleted(OnlineOperationPtr onlineOperationPtr) = 0;
};

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ONLINE_LISTENER
