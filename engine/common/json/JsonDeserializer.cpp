#include "JsonDeserializer.h"

JsonDeserializer::JsonDeserializer(const char* json)
{
	document.Parse(json);
}

bool JsonDeserializer::GetBool(const char* key) const
{
	const rapidjson::Value& value = document[key];
	return value.GetBool();
}

uint32_t JsonDeserializer::GetUint32(const char* key) const
{
	const rapidjson::Value& value = document[key];
	return value.GetUint();
}

float JsonDeserializer::GetFloat(const char* key) const
{
	const rapidjson::Value& value = document[key];
	return value.GetFloat();
}
