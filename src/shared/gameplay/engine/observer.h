#ifndef SHARED_GAMEPLAY_ENGINE_OBSERVER
#define SHARED_GAMEPLAY_ENGINE_OBSERVER

#include "shared/gameplay/gameplay_base.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Event;

class Observer
{
public:

	Observer() {}
	virtual ~Observer() {}

	virtual void OnNotify(Event* event) = 0;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_OBSERVER