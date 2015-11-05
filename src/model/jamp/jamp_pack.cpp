#include "jamp_pack.h"


int32 JampPack::Deserialize(std::string jsonSrc)
{
	if (jsonSrc.length() == 0)
		return 0;

	Reader reader;
	Value value;

	reader.parse(jsonSrc, value, false);

	return DeserializeFromValue(value);
}

int32 JampPack::DeserializeFromValue(Value src){
	if (!src["packId"].empty())
		packId = src["packId"].asInt64();

	if (!src["starsWon"].empty())
		starsWon = src["starsWon"].asUInt64();

	if (!src["starsMax"].empty())
		starsMax = src["starsMax"].asUInt64();
	
	if (!src["blocked"].empty())
		blocked = src["blocked"].asBool();
    
    if (!src["figurationFlag"].empty())
        figurationFlag = src["figurationFlag"].asString();
    
    if (!src["figurationInfo"].empty())
        figurationInfo = src["figurationInfo"].asString();
    
    if (!src["name"].empty())
        name = src["name"].asString();
    
    if (!src["numBlueStarsToUnlock"].empty())
        numBlueStarsToUnlock = src["numBlueStarsToUnlock"].asInt64();
    
    if (!src["timeUnit"].empty())
        timeUnit = src["timeUnit"].asString();

	if (!src["packLevels"].empty())
		packLevels.DeserializeFromValue(src["packLevels"]);
    
    if (!src["packExecution"].empty())
        packExecution = src["packExecution"].asInt64();
    
    if (!src["numberOfLevels"].empty())
        numBlueStarsToUnlock = src["numberOfLevels"].asInt64();
    

	return 0;
}


std::string JampPack::Serialize(){
	FastWriter writer;
	return writer.write(SerializeToValue());
}

Value JampPack::SerializeToValue(){
	Value root;

	root["packId"] = packId;
	root["starsWon"] = starsWon;
	root["starsMax"] = starsMax;
	root["blocked"] = blocked;
    root["figurationFlag"] = figurationFlag;
    root["figurationInfo"] = figurationInfo;
    root["name"] = name;
    root["numBlueStarsToUnlock"] = numBlueStarsToUnlock;
    root["timeUnit"] = timeUnit;
    
    root["packExecution"] = packExecution;
    root["numberOfLevels"] = numberOfLevels;
    
    root["packLevels"] = packLevels.SerializeToValue();

	return root;
}
