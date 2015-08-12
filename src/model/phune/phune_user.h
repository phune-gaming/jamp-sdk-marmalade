#pragma once
#ifndef PHUNE_USER
#define PHUNE_USER

#include "json/json.h"
#include "../json_object.h"

#include <string>
using namespace Json;

enum RegisterType
{
	UNREGISTERED,
	REGISTERED
};

enum Gender
{
	G_UNKNOWN,
	MALE,
	FEMALE
};

class PhuneUser:public JsonObject
{
public:
	PhuneUser(){};
	~PhuneUser(){};

	virtual std::string Serialize() override;
	virtual Value SerializeToValue() override;
	virtual int32 Deserialize(std::string) override;
	virtual int32 DeserializeFromValue(Json::Value src) override;

public:
	int64 id;
	int64 registerDate;
	std::string avatar;
	RegisterType type;
	int64 credits;
	Gender gender;
	std::string name;
	std::string nickname;
	std::string xmppPassword;
	std::string xmppJID;
	float progress;
};
#endif // !PHUNE_USER