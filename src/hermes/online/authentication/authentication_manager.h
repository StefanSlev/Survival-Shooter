#ifndef HERMES_ONLINE_AUTHENTICATION_MANAGER
#define HERMES_ONLINE_AUTHENTICATION_MANAGER

#include "hermes/online/authentication/authentication_base.h"
#include "hermes/online/authentication/authentication_types.h"
#include "hermes/online/online_manager.h"
#include "hermes/system/singleton.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

BEGIN_AUTHENTICATION_NAMESPACE

class AuthenticationManager : public OnlineManager, public system::Singleton<AuthenticationManager>
{
	SINGLETON_ACCESS(AuthenticationManager)
public:
	void Update() override;
	void Authenticate(const AuthenticationInfo& authenticationInfo);

	void OnOperationCompleted(OnlineOperationPtr onlineOperationPtr);
private:

	AuthenticationManager() {}
	~AuthenticationManager() override {}
};

END_AUTHENTICATION_NAMESPACE

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE

#define GetAuthenticationManager() hermes::online::authentication::AuthenticationManager::GetInstance()

#endif // HERMES_ONLINE_AUTHENTICATION_MANAGER