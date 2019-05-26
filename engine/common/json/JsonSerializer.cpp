#include "JsonSerializer.h"

JsonSerializer::JsonSerializer()
	: writer(stringBuffer)
{
	writer.StartObject();
}

void JsonSerializer::WriteBool(const char* key, bool value)
{
	writer.Key(key);
	writer.Bool(value);
}

void JsonSerializer::WriteFloat(const char* key, float value)
{
	writer.Key(key);
	writer.Double(value);
}

void JsonSerializer::WriteUint32(const char* key, uint32_t value)
{
	writer.Key(key);
	writer.Uint(value);
}

void JsonSerializer::WriteString(const char* key, std::string value)
{
	writer.Key(key);
	writer.String(value.c_str());
}

const char* JsonSerializer::ToString()
{
	writer.EndObject();
	return stringBuffer.GetString();
}
