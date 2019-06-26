#include "shared/gameplay/engine/subject.h"
#include "shared/gameplay/engine/observer.h"

BEGIN_SHARED_NAMESPACE

BEGIN_GAMEPLAY_NAMESPACE

void Subject::RegisterObserver(Event::EventType eventType, Observer* observer)
{
	if (!observer)
		return;

	if (observers.find(eventType) == observers.end())
		observers[eventType] = std::vector<Observer*>();
	
	observers[eventType].push_back(observer);
}

bool Subject::UnregisterObserver(Event::EventType eventType, Observer* observer)
{
	if (!observer)
		return false;

	auto it_obsv = observers.find(eventType);

	if (it_obsv == observers.end())
		return false;

	bool found = false;
	auto& eventObservers = it_obsv->second;

	for (auto it = eventObservers.begin(); it != eventObservers.end();)
	{
		if (*it == observer)
		{
			it = eventObservers.erase(it);
			found = true;
		}
		else
			it++;
	}

	return found;
}


bool Subject::UnregisterObserver(Observer* observer)
{
	if (!observer)
		return false;

	bool found = false;

	for (auto& entry : observers)
	{
		Event::EventType eventType = entry.first;
		
		found = this->UnregisterObserver(eventType, observer);

		if (found)
			break;
	}
	 
	return found;
}

void Subject::NotifyObservers(Event* event)
{
	if (!event)
		return;

	Event::EventType eventType = event->eventType;

	auto it_obsv = observers.find(eventType);

	if (it_obsv == observers.end())
		return;

	const auto& eventObservers = it_obsv->second;

	for (const auto& observer : eventObservers)
		observer->OnNotify(event);
}

END_GAMEPLAY_NAMESPACE

END_SHARED_NAMESPACE