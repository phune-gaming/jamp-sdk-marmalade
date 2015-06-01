#pragma once
#ifndef PHUNE_PREFERENCE_BASE64
#define PHUNE_PREFERENCE_BASE64
#include "phune_preference.h"
#include "json/json.h"
#include "../base64.h"
#include "../json_object.h"
#include <string>

class PhunePreferenceBase64 : public PhunePreference
{
public:
	PhunePreferenceBase64();
	~PhunePreferenceBase64();

	std::string Serialize();
	Value SerializeToValue();
	int32 Deserialize(std::string);
	int32 DeserializeFromValue(Json::Value src);
public:
	std::string valueEncoded;
};
#endif // !PHUNE_PREFERENCE_BASE64

