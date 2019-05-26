#ifndef _CE_JSON_SERIALIZER_H_
#define _CE_JSON_SERIALIZER_H_

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include <string>

class JsonSerializer
{
public:
	JsonSerializer();

	void WriteBool(const char* key, bool value);
	void WriteFloat(const char* key, float value);
	void WriteUint32(const char* key, uint32_t value);
	void WriteString(const char* key, std::string value);

	const char* ToString();

private:
	rapidjson::StringBuffer stringBuffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer;
};

#endif // _CE_JSON_SERIALIZER_H_
