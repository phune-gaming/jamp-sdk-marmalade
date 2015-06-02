#include "jamp_pack_level.h"


int32 JampPackLevel::Deserialize(std::string jsonSrc)
{
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

	return 0;
}


std::string JampPackLevel::Serialize(){
	FastWriter writer;
	return writer.write(SerializeToValue());
}

Value JampPackLevel::SerializeToValue(){
	Value root;

	root["packId"] = levelId;
	root["starsWon"] = starsWon;
	root["maxScore"] = maxScore;
	root["blocked"] = blocked;

	return root;
}
