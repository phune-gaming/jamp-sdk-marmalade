#pragma once
#ifndef PHUNE_PREFERENCE
#define PHUNE_PREFERENCE

#include "json/json.h"
#include "../json_object.h"
#include <string>

using namespace Json;

class PhunePreference:public JsonObject
{
public:
	PhunePreference();
	~PhunePreference();

	std::string Serialize();
	Value SerializeToValue();
	int32 Deserialize(std::string);
	int32 DeserializeFromValue(Json::Value src);
public:
	std::string key;
	std::string value;
};
#endif // !PHUNE_PREFERENCE