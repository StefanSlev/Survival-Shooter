#ifndef SHARED_GAMEPLAY_ENGINE_EVENT
#define SHARED_GAMEPLAY_ENGINE_EVENT

#include "shared/gameplay/gameplay_base.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Event
{
public:
	enum class EventType : std::uint8_t
	{
		GameEvent
	};

	EventType eventType;
	virtual ~Event() = 0;

protected:
	Event(EventType eventType) : eventType(eventType) {}
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_EVENT