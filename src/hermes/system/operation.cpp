#include "hermes/system/operation.h"

BEGIN_HERMES_NAMESPACE

BEGIN_SYSTEM_NAMESPACE

Operation::Operation()
{
	SetOperationStatus(OperationStatus::Ready);
}

OperationStatus Operation::GetOperationStatus() const
{
	return this->m_operationStatus;
}

void Operation::SetOperationStatus(OperationStatus operationStatus)
{
	this->m_operationStatus = operationStatus;
}

END_SYSTEM_NAMESPACE

END_HERMES_NAMESPACE