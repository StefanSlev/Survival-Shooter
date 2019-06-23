#ifndef HERMES_SYSTEM_NON_COPYABLE
#define HERMES_SYSTEM_NON_COPYABLE

#include "hermes/system/system_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_SYSTEM_NAMESPACE

class NonCopyable
{
protected:
	NonCopyable() {}
	virtual ~NonCopyable() {}

private:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;
};

END_SYSTEM_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_SYSTEM_NON_COPYABLE