#include "jamp_pack_level.h"


int32 JampPackLevel::Deserialize(std::string jsonSrc)
{
	if (jsonSrc.length() == 0)
		return 0;

	Reader reader;
	Value value;

	reader.parse(jsonSrc, value, false);

	return DeserializeFromValue(value);
}

int32 JampPackLevel::DeserializeFromValue(Value src){
	if (!src["levelId"].empty())
		levelId = src["levelId"].asUInt64();

	if (!src["starsWon"].empty())
		starsWon = src["starsWon"].asUInt64();

	if (!src["maxScore"].empty())
		maxScore = src["maxScore"].asUInt64();
	
	if (!src["blocked"].empty())
		blocked = src["blocked"].asBool();
    
    if (!src["grouping"].empty())
        grouping = src["grouping"].asBool();
    
    if (!src["packId"].empty())
        packId = src["packId"].asInt64();

	return 0;
}


std::string JampPackLevel::Serialize(){
	FastWriter writer;
	return writer.write(SerializeToValue());
}

Value JampPackLevel::SerializeToValue(){
	Value root;

    if(levelId >= 0)
        root["levelId"] = levelId;
    if(starsWon >= 0)
        root["starsWon"] = starsWon;
	if(maxScore >= 0)
        root["maxScore"] = maxScore;
	root["blocked"] = blocked;
    root["grouping"] = grouping;
    if(packId >= 0)
        root["packId"] = packId;

	return root;
}
