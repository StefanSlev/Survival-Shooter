#ifndef HERMES_JSON_FIELD_WRAPPER
#define HERMES_JSON_FIELD_WRAPPER

#include "hermes/json/json_base.h"
#include "hermes/json/json_field_wrapper_base.h"

BEGIN_HERMES_NAMESPACE

BEGIN_JSON_NAMESPACE

template <class FieldType> class JsonFieldWrapper : public JsonFieldWrapperBase
{
public:
	JsonFieldWrapper(const std::string& fieldKey, FieldType& fieldToWrap);
	JsonFieldWrapper(std::string&& fieldKey, FieldType& fieldToWrap);

	FieldType& GetWrappedField() const;

	void ToJson(json& data) const override;
	void FromJson(const json& data) override;

private:
	std::reference_wrapper<FieldType> m_fieldWrapper;
};

template <class FieldType> 
JsonFieldWrapper<FieldType>::JsonFieldWrapper(const std::string& fieldKey, FieldType& fieldToWrap) :
	JsonFieldWrapperBase(fieldKey), m_fieldWrapper{ fieldToWrap } {}

template <class FieldType>
JsonFieldWrapper<FieldType>::JsonFieldWrapper(std::string&& fieldKey, FieldType& fieldToWrap) :
	JsonFieldWrapperBase(std::move(fieldKey)), m_fieldWrapper{ fieldToWrap } {}

template <class FieldType>
FieldType& JsonFieldWrapper<FieldType>::GetWrappedField() const
{
	return this->m_fieldWrapper.get();
}

template <class FieldType>
void JsonFieldWrapper<FieldType>::ToJson(json& data) const
{
	data[m_fieldKey] = GetWrappedField();
}

template <class FieldType>
void JsonFieldWrapper<FieldType>::FromJson(const json& data)
{
	data.at(m_fieldKey).get_to(GetWrappedField());
}

template <class FieldType>
inline std::unique_ptr<JsonFieldWrapperBase> MakeJsonFieldWrapperPtr(const std::string& fieldKey, FieldType& fieldToWrap)
{
	return std::make_unique<JsonFieldWrapper<FieldType>>(fieldKey, fieldToWrap);
}

template <class FieldType>
inline std::unique_ptr<JsonFieldWrapperBase> MakeJsonFieldWrapperPtr(std::string&& fieldKey, FieldType& fieldToWrap)
{
	return std::make_unique<JsonFieldWrapper<FieldType>>(std::move(fieldKey), fieldToWrap);
}

END_JSON_NAMESPACE

END_HERMES_NAMESPACE

#endif // HERMES_JSON_FIELD_WRAPPER