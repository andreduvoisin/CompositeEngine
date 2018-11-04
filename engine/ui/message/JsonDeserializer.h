#ifndef _CE_JSON_DESERIALIZER_H_
#define _CE_JSON_DESERIALIZER_H_

#include "rapidjson/document.h"

class JsonDeserializer
{
public:
	JsonDeserializer(const char* json);

	uint32_t GetUint32(const char* key) const;
	float GetFloat(const char* key) const;

private:
	rapidjson::Document document;
};

#endif // _CE_JSON_DESERIALIZER_H_
