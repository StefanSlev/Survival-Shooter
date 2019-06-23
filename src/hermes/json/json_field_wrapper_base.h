#ifndef HERMES_JSON_FIELD_WRAPPER_BASE
#define HERMES_JSON_FIELD_WRAPPER_BASE

#include "hermes/json/json_base.h"
#include "hermes/system/non_copyable.h"

BEGIN_HERMES_NAMESPACE

BEGIN_JSON_NAMESPACE

class JsonFieldWrapperBase : public system::NonCopyable
{
public:
	virtual void ToJson(json& data) const = 0;
	virtual void FromJson(const json& data) = 0;

protected:
	JsonFieldWrapperBase(const std::string& fieldKey);
	JsonFieldWrapperBase(std::string&& fieldKey);

	std::string m_fieldKey;
};

END_JSON_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_JSON_FIELD_WRAPPER_BASE
