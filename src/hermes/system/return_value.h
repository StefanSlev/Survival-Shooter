#ifndef HERMES_SYSTEM_RETURN_VALUE
#define HERMES_SYSTEM_RETURN_VALUE

#include "hermes/system/system_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_SYSTEM_NAMESPACE

template<class ReturnType>
using ReturnValuePtr = std::shared_ptr<ReturnType>;

template<class ReturnType, class... ReturnTypeArgs>
inline ReturnValuePtr<ReturnType> MakeReturnValuePtr(ReturnTypeArgs&&... returnTypeArgs)
{
	return std::make_shared<ReturnType>(std::forward<ReturnTypeArgs>(returnTypeArgs)...);
}

END_SYSTEM_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_SYSTEM_RETURN_VALUE