#ifndef HERMES_ASYNC_BASE
#define HERMES_ASYNC_BASE

#include "hermes/hermes_base.h"

BEGIN_HERMES_NAMESPACE

#define BEGIN_ASYNC_NAMESPACE namespace async {
#define END_ASYNC_NAMESPACE }

BEGIN_ASYNC_NAMESPACE

using AsyncHandler = std::future<void>;

END_ASYNC_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_ASYNC_BASE