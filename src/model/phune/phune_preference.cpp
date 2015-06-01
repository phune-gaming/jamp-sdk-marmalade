#include "phune_preference.h"

PhunePreference::PhunePreference()
{
	
}


PhunePreference::~PhunePreference()
{
}

int32 PhunePreference::Deserialize(std::string jsonSrc)
{
	Reader reader;
	Value jsonValue;

	reader.parse(jsonSrc, jsonValue, false);

	return DeserializeFromValue(jsonValue);
}

int32 PhunePreference::DeserializeFromValue(Json::Value src){
	if (!src["key"].empty())
		key = src["key"].asString();

	if (!src["value"].empty())
	{
		value = src["value"].asString();
	}
	return 0;
}

std::string PhunePreference::Serialize(){
	FastWriter writer;
	return writer.write(SerializeToValue());
}

Value PhunePreference::SerializeToValue(){
	Value root;
	root["key"] = key;
	root["value"] = value;
	return root;
}
