#include "JsonDeserializer.h"

JsonDeserializer::JsonDeserializer(const char* json)
{
	document.Parse(json);
}

unsigned JsonDeserializer::GetUnsigned(const char* key)
{
	const rapidjson::Value& value = document[key];
	return value.GetUint();
}
