#include "hermes/online/authentication/authentication_manager.h"
#include "hermes/online/authentication/authentication_operations.h"
#include "hermes/online/online_worker.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

BEGIN_AUTHENTICATION_NAMESPACE

void AuthenticationManager::Update()
{
}

void AuthenticationManager::Authenticate(const AuthenticationInfo& authenticationInfo)
{
	hermes::online::OnlineOperation* op = new AuthenticateOperation(authenticationInfo);
	GetOnlineWorker().RegisterOnlineOperation(OnlineOperationPtr(op), this);
}

void AuthenticationManager::OnOperationCompleted(OnlineOperationPtr onlineOperationPtr)
{
	if (onlineOperationPtr->GetOnlineOperationType() == hermes::online::OnlineOperationType::Authenticate)
	{
		if (onlineOperationPtr->GetOperationStatus() == hermes::system::OperationStatus::Succeeded)
		{
			
		}
		else
		{
			
		}
	}
}

END_AUTHENTICATION_NAMESPACE

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE