#pragma once
#ifndef PHUNE_MATCH
#define PHUNE_MATCH

#include "phune_match.h"
#include "json/json.h"
#include "../json_object.h"
#include "../json_list_object.h"
#include "phune_player.h"


#include <string>
using namespace Json;

class PhuneMatch:public JsonObject
{
public:

	virtual std::string Serialize() override;
	virtual Value SerializeToValue() override;
	virtual int32 Deserialize(std::string) override;
	virtual int32 DeserializeFromValue(Json::Value src) override;

public:
	int64 matchId;
	int64 playerId;
};
#endif // !PHUNE_MATCH