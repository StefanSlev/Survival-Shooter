#ifndef HERMES_ONLINE_AUTHENTICATION_BASE
#define HERMES_ONLINE_AUTHENTICATION_BASE

#include "hermes/online/online_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

#define BEGIN_AUTHENTICATION_NAMESPACE namespace authentication {
#define END_AUTHENTICATION_NAMESPACE }

BEGIN_AUTHENTICATION_NAMESPACE

using ProfileId = std::uint64_t;

END_AUTHENTICATION_NAMESPACE

END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ONLINE_AUTHENTICATION_BASE