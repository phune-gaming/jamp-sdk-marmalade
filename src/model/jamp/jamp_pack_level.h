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
	int64 levelId = -1;
	//Stars won by the player in level
	int64 starsWon = -1;
	
	int64 maxScore = -1;

	//level blocked
	bool blocked;
    
    bool grouping;
    
    int64 packId = -1;

};

#endif // !JAMP_PACK_LEVEL

