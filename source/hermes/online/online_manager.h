#ifndef HERMES_ONLINE_MANAGER
#define HERMES_ONLINE_MANAGER

#include "hermes/online/online_base.h"
#include "hermes/online/online_listener.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

class OnlineManager : public OnlineListener
{
public:
	virtual void Update() {}
protected:
	void OnOperationCompleted(OnlineOperationPtr onlineOperationPtr) {}
};

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ONLINE_MANAGER