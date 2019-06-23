#ifndef HERMES_ONLINE_AUTHENTICATION_OPERATIONS
#define HERMES_ONLINE_AUTHENTICATION_OPERATIONS

#include "hermes/online/authentication/authentication_base.h"
#include "hermes/online/authentication/authentication_types.h"
#include "hermes/online/authentication/authentication_return_values.h"
#include "hermes/online/online_operation.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

BEGIN_AUTHENTICATION_NAMESPACE

class AuthenticateOperation : public OnlineOperation
{
public:
	AuthenticateOperation(const AuthenticationInfo& authenticationInfo)
	{
		hermes::json::json data = authenticationInfo;

		m_session.SetUrl({ "" });
		m_session.SetHeader({ {"Content-Type", "application/json"} });
		m_session.SetBody(cpr::Body{ data.dump() });

		this->m_onlineOperationMethod = hermes::online::OnlineOperationMethod::HttpPost;

		auto authenticationResultPtr = hermes::system::MakeReturnValuePtr<AuthenticationResult>();
		auto serializablePtr = std::static_pointer_cast<hermes::json::JsonSerializable> (authenticationResultPtr);

		SetAsyncReturnValue(hermes::async::MakeAsyncReturnValue(serializablePtr));
	}

	hermes::online::OnlineOperationType GetOnlineOperationType() const override
	{
		return hermes::online::OnlineOperationType::Authenticate;
	}
};

END_AUTHENTICATION_NAMESPACE

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ONLINE_AUTHENTICATION_OPERATIONS