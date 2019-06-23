#include "hermes/json/json_serializable.h"

BEGIN_HERMES_NAMESPACE

BEGIN_JSON_NAMESPACE

void to_json(json& data, const JsonSerializable& serializable)
{
	for (const auto& jsonField : serializable.m_jsonFields)
	{
		jsonField->ToJson(data);
	}
}

void from_json(const json& data, JsonSerializable& serializable)
{
	for (const auto& jsonField : serializable.m_jsonFields)
	{
		jsonField->FromJson(data);
	}
}

END_JSON_NAMESPACE

END_HERMES_NAMESPACE