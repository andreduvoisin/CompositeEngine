#ifndef _CE_JSON_DESERIALIZER_H_
#define _CE_JSON_DESERIALIZER_H_

#include "rapidjson/document.h"

class JsonDeserializer
{
public:
	JsonDeserializer(const char* json);

	unsigned GetUnsigned(const char* key);

private:
	rapidjson::Document document;
};

#endif // _CE_JSON_DESERIALIZER_H_
