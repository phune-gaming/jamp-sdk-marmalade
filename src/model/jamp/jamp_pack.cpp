#include "jamp_pack.h"


int32 JampPack::Deserialize(std::string jsonSrc)
{
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

	if (!src["packLevels"].empty())
		packLevels.DeserializeFromValue(src["packLevels"]);
	

	
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

	root["packLevels"] = packLevels.SerializeToValue();

	return root;
}
