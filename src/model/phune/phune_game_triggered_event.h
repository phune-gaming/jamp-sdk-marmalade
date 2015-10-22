#pragma once
#ifndef PHUNE_GAME_TRIGGERED_EVENT
#define PHUNE_GAME_TRIGGERED_EVENT

#include "json/json.h"
#include "../json_object.h"

#include <string>
using namespace Json;

class GameTriggerdEvent:public JsonObject
{
public:
    GameTriggerdEvent(){};
    ~GameTriggerdEvent(){};
    
	virtual std::string Serialize() override;
	virtual Value SerializeToValue() override;
	virtual int32 Deserialize(std::string) override;
	virtual int32 DeserializeFromValue(Json::Value src) override;

public:
    int64 playerId;
    std::string type;
    std::string data;
    
};
#endif // !PHUNE_GAME_TRIGGERED_EVENT