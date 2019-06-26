#ifndef HERMES_NETWORK_BASE
#define HERMES_NETWORK_BASE

#include "hermes/hermes_base.h"
#include <yojimbo/yojimbo.h>

BEGIN_HERMES_NAMESPACE

#define BEGIN_NETWORK_NAMESPACE namespace network {
#define END_NETWORK_NAMESPACE }

BEGIN_NETWORK_NAMESPACE

using NetworkId = std::uint32_t;
constexpr NetworkId InvalidNetworkId = UINT32_MAX;

using ObjectId = std::uint32_t;

constexpr int MaxMessageSize = 1024;
using NetworkMessageData = std::uint8_t[MaxMessageSize];

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_BASE