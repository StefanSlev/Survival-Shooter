#include "hermes/online/online_worker.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

OnlineWorker::~OnlineWorker()
{
	m_registeredOperations.clear();
}

void OnlineWorker::Update()
{
	for (auto it = m_registeredOperations.begin(); it != m_registeredOperations.end();)
	{
		auto handlerStatus = it->m_operationHandler.wait_for(std::chrono::milliseconds(0));

		if (handlerStatus == std::future_status::ready)
		{
			it->m_operationHandler.get();

			if (it->m_onlineListener != nullptr)
			{
				it->m_onlineListener->OnOperationCompleted(std::move(it->m_onlineOperation));
			}

			it = m_registeredOperations.erase(it);
		}
		else
			++it;
	}
}

void OnlineWorker::RegisterOnlineOperation(OnlineOperationPtr onlineOperationPtr, OnlineListener* onlineListener)
{
	if (onlineOperationPtr)
	{
		auto operationHandler = onlineOperationPtr->Start();

		if (operationHandler.valid())
		{
			m_registeredOperations.push_back({

				std::move(onlineOperationPtr),
				std::move(operationHandler),
				onlineListener
			});
		}
	}
}

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE