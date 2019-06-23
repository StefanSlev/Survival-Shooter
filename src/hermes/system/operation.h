#ifndef HERMES_SYSTEM_OPERATION
#define HERMES_SYSTEM_OPERATION

#include "hermes/system/system_base.h"
#include "hermes/system/operation_status.h"
#include "hermes/system/non_copyable.h"

BEGIN_HERMES_NAMESPACE

BEGIN_SYSTEM_NAMESPACE

class Operation : public NonCopyable
{
public:
	Operation();

	OperationStatus GetOperationStatus() const;

protected:
	virtual void StartInternal() = 0;
	void SetOperationStatus(OperationStatus operationStatus);

private:
	OperationStatus m_operationStatus;
};

END_SYSTEM_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_SYSTEM_OPERATION