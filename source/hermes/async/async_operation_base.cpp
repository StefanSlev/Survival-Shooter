#include "hermes/async/async_operation_base.h"
#include "hermes/system/operation_status.h"

BEGIN_HERMES_NAMESPACE

BEGIN_ASYNC_NAMESPACE

AsyncHandler AsyncOperationBase::Start()
{
	if (GetOperationStatus() == system::OperationStatus::Ready)
	{
		return std::async([this]() -> void {

			this->StartInternal();
		});
	}
	
	return AsyncHandler();
}

END_ASYNC_NAMESPACE

END_HERMES_NAMESPACE