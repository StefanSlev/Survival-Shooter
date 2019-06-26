#ifndef HERMES_JSON_SERIALIZABLE
#define HERMES_JSON_SERIALIZABLE

#include "hermes/json/json_base.h"
#include "hermes/json/json_field_wrapper.h"

BEGIN_HERMES_NAMESPACE

BEGIN_JSON_NAMESPACE

class JsonSerializable
{
public:
	friend void to_json(json& data, const JsonSerializable& serializable);
	friend void from_json(const json& data, JsonSerializable& serializable);

protected:
	std::vector<std::unique_ptr<JsonFieldWrapperBase>> m_jsonFields;
};

void to_json(json& data, const JsonSerializable& serializable);
void from_json(const json& data, JsonSerializable& serializable);

END_JSON_NAMESPACE

END_HERMES_NAMESPACE

#define REGISTER_JSON_FIELD(FIELD_NAME, FIELD_VALUE) \
m_jsonFields.push_back(hermes::json::MakeJsonFieldWrapperPtr(FIELD_NAME, FIELD_VALUE));

#endif // HERMES_JSON_SERIALIZABLE