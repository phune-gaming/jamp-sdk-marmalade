#pragma once
#ifndef JAMP_PACK_LEVEL
#define JAMP_PACK_LEVEL

#include "json/json.h"
#include "../json_object.h"

#include <string>

using namespace Json;

class JampPackLevel :
	public JsonObject
{
public:
	virtual std::string Serialize() override;
	virtual Json::Value SerializeToValue() override;
	virtual int32 Deserialize(std::string) override;
	virtual int32 DeserializeFromValue(Json::Value src) override;

public:
	//The id of the level
	int64 levelId;
	//Stars won by the player in level
	int64 starsWon;
	
	int64 maxScore;

	//level blocked

	bool blocked;
};

#endif // !JAMP_PACK_LEVEL

