#ifndef HERMES_ASYNC_OPERATION_BASE
#define HERMES_ASYNC_OPERATION_BASE

#include "hermes/async/async_base.h"
#include "hermes/system/operation.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ASYNC_NAMESPACE

class AsyncOperationBase : public system::Operation
{
public:
	AsyncHandler Start();
};

END_ASYNC_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ASYNC_OPERATION_BASE