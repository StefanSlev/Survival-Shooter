#ifndef SHARED_GAMEPLAY_ENGINE_SUBJECT
#define SHARED_GAMEPLAY_ENGINE_SUBJECT

#include "shared/gameplay/gameplay_base.h"
#include "shared/gameplay/engine/event.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

class Observer;

class Subject {

public:
	Subject() {}
	virtual ~Subject() {}

	void RegisterObserver(Event::EventType eventType, Observer* observer);

	bool UnregisterObserver(Event::EventType eventType, Observer* observer);
	bool UnregisterObserver(Observer* observer);

	void NotifyObservers(Event* event);

private:
	std::unordered_map<Event::EventType, std::vector<Observer*>> observers;
};

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE

#endif // SHARED_GAMEPLAY_ENGINE_SUBJECT