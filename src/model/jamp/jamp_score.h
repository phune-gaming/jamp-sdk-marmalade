#pragma once
#ifndef JAMP_SCORE
#define JAMP_SCORE

#include "json/json.h"
#include "../json_object.h"
#include "../json_list_object.h"
#include "jamp_cell_performance.h"

#include <string>

using namespace Json;

class JampScore :
	public JsonObject
{
public:
	virtual std::string Serialize() override;
	virtual Json::Value SerializeToValue() override;
	virtual int32 Deserialize(std::string) override;
	virtual int32 DeserializeFromValue(Json::Value src) override;

public:
	//The final score of a match in points
	int64 score;
	//Time spent of the match
	int64 timeSpent;
	//Time stamp of the server (automatic)
	int64 timeStamp;
	//The match id (automatic)
	int64 matchId;

	//cells performances
	JsonListObject<JampCellPerformance> cellsPerformance;

};

#endif // !JAMP_SCORE

