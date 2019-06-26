#ifndef HERMES_JSON_BASE
#define HERMES_JSON_BASE

#include "hermes/hermes_base.h"
#include <nlohmann/json.hpp>

BEGIN_HERMES_NAMESPACE

#define BEGIN_JSON_NAMESPACE namespace json {
#define END_JSON_NAMESPACE }

BEGIN_JSON_NAMESPACE

using json = nlohmann::json;

END_JSON_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_JSON_BASE