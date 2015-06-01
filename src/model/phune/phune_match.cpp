#include "phune_match.h"


int32 PhuneMatch::Deserialize(std::string jsonSrc)
{
	Reader reader;
	Value value;

	reader.parse(jsonSrc, value, false);

	return DeserializeFromValue(value);
}

int32 PhuneMatch::DeserializeFromValue(Json::Value src){
	if (!src["matchId"].empty())
		matchId = src["matchId"].asUInt64();

	if (!src["playerId"].empty())
		playerId = src["playerId"].asUInt64();
	
	return 0;
}


std::string PhuneMatch::Serialize(){
	FastWriter writer;
	return writer.write(SerializeToValue());
}

Value PhuneMatch::SerializeToValue(){
	Value root;

	root["matchId"] = matchId;

	root["playerId"] = playerId;

	return root;
}
