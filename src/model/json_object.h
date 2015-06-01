#pragma once
#ifndef JSON_OBJECT
#define JSON_OBJECT

#include <s3e.h>
#include <string>
#include "json/json.h"

class JsonObject
{
public:
	virtual std::string Serialize() = 0;
	virtual Json::Value SerializeToValue() = 0;
	virtual int32 Deserialize(std::string src) = 0;
	virtual int32 DeserializeFromValue(Json::Value src) = 0;
};
#endif //!JSON_OBJECT

