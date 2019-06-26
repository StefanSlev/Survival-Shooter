#ifndef HERMES_ASYNC_OPERATION_TYPE
#define HERMES_ASYNC_OPERATION_TYPE

#include "hermes/async/async_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ASYNC_NAMESPACE

/*
 *	@brief
 *	AsyncOperationType: Types of registered async operation
 *  - Add custom type when create new operation
 */

enum class AsyncOperationType : std::uint8_t
{
	OnlineOperation
};

END_ASYNC_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ASYNC_OPERATION_TYPE
