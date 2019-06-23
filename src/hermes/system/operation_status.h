#ifndef HERMES_SYSTEM_OPERATION_STATUS
#define HERMES_SYSTEM_OPERATION_STATUS

#include "hermes/system/system_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_SYSTEM_NAMESPACE

enum class OperationStatus : std::uint8_t
{
	Ready,
	Succeeded,
	Failed
};

END_SYSTEM_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_SYSTEM_OPERATION_STATUS