#ifndef HERMES_ONLINE_AUTHENTICATION_RETURN_VALUES
#define HERMES_ONLINE_AUTHENTICATION_RETURN_VALUES

#include "hermes/online/authentication/authentication_base.h"
#include "hermes/json/json_serializable.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

BEGIN_AUTHENTICATION_NAMESPACE

struct AuthenticationResult : public json::JsonSerializable
{
	ProfileId m_profileId;

	AuthenticationResult()
	{
		REGISTER_JSON_FIELD("profileId", m_profileId)
	}
};

END_AUTHENTICATION_NAMESPACE

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ONLINE_AUTHENTICATION_RETURN_VALUES