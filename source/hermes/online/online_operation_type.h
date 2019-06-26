#ifndef HERMES_ONLINE_OPERATION_TYPE
#define HERMES_ONLINE_OPERATION_TYPE

#include "hermes/online/online_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ONLINE_NAMESPACE

/*
 *	@brief
 *	OnlineOperationType: Types of registered online operation
 *  - Add custom type when create new operation
 */

enum class OnlineOperationType : std::uint8_t
{
	Invalid,
	GetLeaderboard,
	AddLeaderboardEntry
};


END_ONLINE_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ONLINE_OPERATION_TYPE