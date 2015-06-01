#pragma once
#ifndef PHUNE_PLAYER
#define PHUNE_PLAYER

#include "json/json.h"
#include "../json_object.h"

#include <string>
using namespace Json;

enum PlayerStatus
{
	WINNER,
	LOOSER
};

class PhunePlayer:public JsonObject
{
public:
	PhunePlayer();
	~PhunePlayer();

	virtual std::string Serialize() override;
	virtual Value SerializeToValue() override;
	virtual int32 Deserialize(std::string) override;
	virtual int32 DeserializeFromValue(Json::Value src) override;

public:
	int64 id;
	int64 score;
	PlayerStatus status;
};
#endif // !PHUNE_Player