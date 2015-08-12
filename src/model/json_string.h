#pragma once
#ifndef JSON_STRING
#define JSON_STRING

#include "json/json.h"
#include "json_object.h"
#include "json_list_object.h"
#include <IwUtil.h>


#include <string>
using namespace Json;

class JsonString:public JsonObject
{
public:

	virtual std::string Serialize() override;
	virtual Value SerializeToValue() override;
	virtual int32 Deserialize(std::string) override;
	virtual int32 DeserializeFromValue(Json::Value src) override;

public:
	std::string value;
};
#endif // !JSON_STRING