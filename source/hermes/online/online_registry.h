#ifndef HERMES_ONLINE_REGISTRY
#define HERMES_ONLINE_REGISTRY

#include "hermes/online/online_base.h"
#include "hermes/online/online_manager.h"
#include "hermes/system/singleton.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

class OnlineRegistry : public system::Singleton<OnlineRegistry>
{
	SINGLETON_ACCESS(OnlineRegistry)
public:
	
	void Update();
	void Register(OnlineManager* onlineManager);
	void Unregister(OnlineManager* onlineManager);

private:
	std::list<OnlineManager*> m_onlineManagers;

	OnlineRegistry() {}
	~OnlineRegistry() override {}
};

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE

#define GetOnlineRegistry() hermes::online::OnlineRegistry::GetInstance()

#endif // HERMES_ONLINE_REGISTRY