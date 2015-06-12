#include "phune_preference_base64.h"


PhunePreferenceBase64::PhunePreferenceBase64()
{
}


PhunePreferenceBase64::~PhunePreferenceBase64()
{
}

int32 PhunePreferenceBase64::Deserialize(std::string jsonSrc)
{

	if (jsonSrc.length() == 0)
		return 0;

	Reader reader;
	Value jsonValue;

	reader.parse(jsonSrc, jsonValue, false);

	return DeserializeFromValue(jsonValue);
}

int32 PhunePreferenceBase64::DeserializeFromValue(Json::Value src){
	if (!src["key"].empty())
		key = src["key"].asString();

	if (!src["value"].empty())
	{
		valueEncoded = src["value"].asString();
		value = base64_decode(valueEncoded);
	}

	return 0;
}

std::string PhunePreferenceBase64::Serialize(){
	FastWriter writer;
	return writer.write(SerializeToValue());
}

Value PhunePreferenceBase64::SerializeToValue(){
	Value root;

	root["key"] = key;
	root["value"] = valueEncoded;

	return root;
}
