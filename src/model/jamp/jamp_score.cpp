#include "jamp_score.h"


int32 JampScore::Deserialize(std::string jsonSrc)
{
	Reader reader;
	Value value;

	reader.parse(jsonSrc, value, false);

	return DeserializeFromValue(value);
}

int32 JampScore::DeserializeFromValue(Value src){
	if (!src["score"].empty())
		score = src["score"].asUInt64();

	if (!src["timeSpent"].empty())
		timeSpent = src["timeSpent"].asUInt64();

	if (!src["timeStamp"].empty())
		timeStamp = src["timeStamp"].asUInt64();
	
	if (!src["matchId"].empty())
		matchId = src["matchId"].asUInt64();

	if (!src["cellsPerformance"].empty())
		cellsPerformance.DeserializeFromValue(src["cellsPerformance"]);
	

	
	return 0;
}


std::string JampScore::Serialize(){
	FastWriter writer;
	return writer.write(SerializeToValue());
}

Value JampScore::SerializeToValue(){
	Value root;

	root["score"] = score;
	root["timeSpent"] = timeSpent;
	root["timeStamp"] = timeStamp;
	root["matchId"] = matchId;

	root["cellsPerformance"] = cellsPerformance.SerializeToValue();

	return root;
}
