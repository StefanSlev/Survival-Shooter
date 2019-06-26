#include "hermes/json/json_field_wrapper_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_JSON_NAMESPACE

JsonFieldWrapperBase::JsonFieldWrapperBase(const std::string& fieldKey)
{
	this->m_fieldKey = fieldKey;
}

JsonFieldWrapperBase::JsonFieldWrapperBase(std::string&& fieldKey)
{
	this->m_fieldKey = std::move(fieldKey);
}

END_JSON_NAMESPACE

END_HERMES_NAMESPACE