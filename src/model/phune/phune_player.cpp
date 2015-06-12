#include "phune_player.h"

PhunePlayer::PhunePlayer()
{
	
}


PhunePlayer::~PhunePlayer()
{
}


int32 PhunePlayer::Deserialize(std::string jsonSrc)
{
	if (jsonSrc.length() == 0)
		return 0;
	
	Reader reader;
	Value value;

	reader.parse(jsonSrc, value, false);

	return DeserializeFromValue(value);
}

int32 PhunePlayer::DeserializeFromValue(Json::Value src){
	if (!src["id"].empty())
		id = src["id"].asUInt64();

	if (!src["score"].empty())
		score = src["score"].asUInt64();

	if (!src["status"].empty())
	{
		std::string typeStr = src["status"].asString();
		if (typeStr.compare("WINNER") == 0)
			status = WINNER;
		else
			status = LOOSER;
	}
	return 0;
}


std::string PhunePlayer::Serialize(){
	FastWriter writer;
	return writer.write(SerializeToValue());
}

Value PhunePlayer::SerializeToValue(){
	Value root;

	root["id"] = id;

	root["score"] = score;
	
	switch (status){
	case WINNER:
		root["status"] = "WINNER";
		break;
	default:
	case LOOSER:
		root["status"] = "LOOSER";
		break;
	}
	return root;
}
