#ifndef HERMES_ASYNC_OPERATION
#define HERMES_ASYNC_OPERATION

#include "hermes/async/async_base.h"
#include "hermes/async/async_operation_base.h"
#include "hermes/async/async_operation_type.h"
#include "hermes/async/async_return_value.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ASYNC_NAMESPACE

template <class ReturnType>
class AsyncOperation : public AsyncOperationBase
{
public:
	virtual AsyncOperationType GetAsyncOperationType() const = 0;
	
	const AsyncReturnValue<ReturnType>& GetAsyncReturnValue() const
	{
		return this->m_asyncReturnValue;
	}

	void SetAsyncReturnValue(const AsyncReturnValue<ReturnType>& asyncReturnValue)
	{
		this->m_asyncReturnValue = asyncReturnValue;
	}

private:
	AsyncReturnValue<ReturnType> m_asyncReturnValue;
};

END_ASYNC_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ASYNC_OPERATION