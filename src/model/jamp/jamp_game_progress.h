#pragma once
#ifndef JAMP_GAME_PROGRESS
#define JAMP_GAME_PROGRESS

#include "json/json.h"
#include "../json_object.h"


using namespace Json;

class JampGameProgress :
	public JsonObject
{
public:
	virtual std::string Serialize() override;
	virtual Json::Value SerializeToValue() override;
	virtual int32 Deserialize(std::string) override;
	virtual int32 DeserializeFromValue(Json::Value src) override;

public:
	//percentages
	float progress = -1;
	

};

#endif // !JAMP_GAME_PROGRESS

