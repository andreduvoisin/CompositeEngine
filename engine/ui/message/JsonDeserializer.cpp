#include "JsonDeserializer.h"

JsonDeserializer::JsonDeserializer(const char* json)
{
	document.Parse(json);
}

uint32_t JsonDeserializer::GetUint32(const char* key) const
{
	const rapidjson::Value& value = document[key];
	return value.GetUint();
}
