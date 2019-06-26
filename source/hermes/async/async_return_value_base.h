#ifndef HERMES_ASYNC_RETURN_VALUE_BASE
#define HERMES_ASYNC_RETURN_VALUE_BASE

#include "hermes/async/async_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ASYNC_NAMESPACE

class AsyncReturnValueBase
{
public:
	virtual ~AsyncReturnValueBase() = 0;
};

END_ASYNC_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ASYNC_RETURN_VALUE_BASE