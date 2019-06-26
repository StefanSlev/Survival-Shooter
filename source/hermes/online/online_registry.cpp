#include "hermes/online/online_registry.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

void OnlineRegistry::Update()
{
	for (OnlineManager* onlineManager : m_onlineManagers)
	{
		onlineManager->Update();
	}
}

void OnlineRegistry::Register(OnlineManager* onlineManager)
{
	if (onlineManager == nullptr)
		return;

	bool alreadyRegistered{ false };

	for (OnlineManager* registeredManager : m_onlineManagers)
	{
		if (registeredManager == onlineManager)
		{
			alreadyRegistered = true;
			break;
		}
	}

	if (!alreadyRegistered)
		m_onlineManagers.push_back(onlineManager);
}

void OnlineRegistry::Unregister(OnlineManager* onlineManager)
{
	if (onlineManager == nullptr)
		return;

	for (auto it = m_onlineManagers.begin(); it != m_onlineManagers.end();)
	{
		if (*it == onlineManager)
		{
			it = m_onlineManagers.erase(it);
			break;
		}
		else
			++it;
	}
}

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE