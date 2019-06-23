#ifndef HERMES_ONLINE_AUTHENTICATION_TYPES
#define HERMES_ONLINE_AUTHENTICATION_TYPES

#include "hermes/online/authentication/authentication_base.h"
#include "hermes/json/json_serializable.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

BEGIN_AUTHENTICATION_NAMESPACE

struct AuthenticationInfo : public json::JsonSerializable
{
	std::string m_userName;

	AuthenticationInfo()
	{
		REGISTER_JSON_FIELD("userName", m_userName)
	}
};

END_AUTHENTICATION_NAMESPACE

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ONLINE_AUTHENTICATION_TYPES