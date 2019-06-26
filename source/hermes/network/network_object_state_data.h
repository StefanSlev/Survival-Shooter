#ifndef HERMES_NETWORK_OBJECT_STATE_DATA
#define HERMES_NETWORK_OBJECT_STATE_DATA

#include "hermes/network/network_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_NETWORK_NAMESPACE

using NetworkObjectStateId = std::uint32_t;
using NetworkObjectStateData = yojimbo::Serializable;
using NetworkObjectStateIndex = std::uint64_t;

using NetworkObjectStateDataUPtr = std::unique_ptr<NetworkObjectStateData>;
using NetworkObjectStateDataSPtr = std::shared_ptr<NetworkObjectStateData>;

constexpr NetworkObjectStateId InvalidNetworkObjectStateId = UINT32_MAX;

END_NETWORK_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_NETWORK_OBJECT_STATE_DATA
