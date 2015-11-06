#pragma once
#ifndef JAMP_PACK
#define JAMP_PACK

#include "json/json.h"
#include "../json_object.h"
#include "../json_list_object.h"
#include "jamp_pack_level.h"

#include <string>

using namespace Json;

class JampPack :
	public JsonObject
{
public:
	virtual std::string Serialize() override;
	virtual Json::Value SerializeToValue() override;
	virtual int32 Deserialize(std::string) override;
	virtual int32 DeserializeFromValue(Json::Value src) override;

public:
	//The id of the pack
	int64 packId = -1;
	//Stars won by the player in pack
	int64 starsWon = -1;
	//Maximum stars a player could achieve
	int64 starsMax = -1;
	//Pack blocked
	bool blocked;
    
    std::string figurationFlag;
    std::string figurationInfo;
    std::string name;
    int64 numBlueStarsToUnlock = -1;
    std::string timeUnit;
    
    //Percentage of complete
    int64 packExecution = -1;
    //Total number of levels
    int64 numberOfLevels  = -1;

	//cells performances (ordered)
	JsonListObject<JampPackLevel> packLevels;

};

#endif // !JAMP_PACK

