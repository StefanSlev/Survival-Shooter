#ifndef HERMES_ASYNC_RETURN_VALUE
#define HERMES_ASYNC_RETURN_VALUE

#include "hermes/async/async_base.h"
#include "hermes/async/async_return_value_base.h"
#include "hermes/system/return_value.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ASYNC_NAMESPACE

template <class ReturnType>
class AsyncReturnValue : public AsyncReturnValueBase
{
public:
	AsyncReturnValue(const system::ReturnValuePtr<ReturnType>& returnValuePtr = nullptr)
	{
		SetReturnValuePtr(returnValuePtr);
	}

	void SetReturnValuePtr(const system::ReturnValuePtr<ReturnType>& returnValuePtr)
	{
		this->m_returnValuePtr = returnValuePtr;
	}

	const system::ReturnValuePtr<ReturnType>& GetReturnValuePtr() const
	{
		return this->m_returnValuePtr;
	}

private:
	system::ReturnValuePtr<ReturnType> m_returnValuePtr;
};

template <class ReturnType>
AsyncReturnValue<ReturnType> MakeAsyncReturnValue()
{
	return { system::MakeReturnValuePtr<ReturnType>() };
}

template <class ReturnType>
AsyncReturnValue<ReturnType> MakeAsyncReturnValue(const system::ReturnValuePtr<ReturnType>& returnValuePtr)
{
	return { returnValuePtr };
}

END_ASYNC_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ASYNC_RETURN_VALUE