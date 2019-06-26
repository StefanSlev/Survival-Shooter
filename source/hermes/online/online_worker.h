#ifndef HERMES_ONLINE_WORKER
#define HERMES_ONLINE_WORKER

#include "hermes/online/online_base.h"
#include "hermes/system/singleton.h"
#include "hermes/online/online_operation.h"
#include "hermes/online/online_listener.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

class OnlineWorker : public system::Singleton<OnlineWorker>
{
	SINGLETON_ACCESS(OnlineWorker)
public:
	void Update();
	void RegisterOnlineOperation(OnlineOperationPtr onlineOperationPtr, OnlineListener* onlineListener = nullptr);

private:
	struct OnlineOperationInfo
	{
		OnlineOperationPtr m_onlineOperation;
		async::AsyncHandler m_operationHandler;
		OnlineListener* m_onlineListener;
	};

	std::list<OnlineOperationInfo> m_registeredOperations;

	OnlineWorker() {}
	~OnlineWorker();
};

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE

#define GetOnlineWorker() hermes::online::OnlineWorker::GetInstance()

#endif // HERMES_ONLINE_WORKER