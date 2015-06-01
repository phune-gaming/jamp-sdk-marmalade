#include "phune_user.h"


int32 PhuneUser::Deserialize(std::string jsonSrc)
{
	Reader reader;
	Value value;

	reader.parse(jsonSrc, value, false);

	return DeserializeFromValue(value);
}

int32 PhuneUser::DeserializeFromValue(Json::Value src){
	if (!src["id"].empty())
		id = src["id"].asUInt64();

	if (!src["registerDate"].empty())
		registerDate = src["registerDate"].asInt64();

	if (!src["avatar"].empty())
		avatar = src["avatar"].asString();

	if (!src["type"].empty())
	{
		std::string typeStr = src["type"].asString();
		if (typeStr.compare("REGISTERED") == 0)
			type = REGISTERED;
		else
			type = UNREGISTERED;
	}

	if (!src["credits"].empty())
		credits = src["credits"].asUInt64();

	if (!src["gender"].empty())
	{
		std::string genderStr = src["gender"].asString();
		if (genderStr.compare("MALE") == 0)
			gender = MALE;
		else if (genderStr.compare("FEMALE") == 0)
			gender = FEMALE;
		else
			gender = UNKNOWN;
	}

	if (!src["name"].empty())
		name = src["name"].asString().c_str();

	if (!src["nickname"].empty())
		nickname = src["nickname"].asString();

	if (!src["xmppPassword"].empty())
		xmppPassword = src["xmppPassword"].asString();

	if (!src["xmppJID"].empty())
		xmppJID = src["xmppJID"].asString();

	if (!src["progress"].empty())
		progress = src["progress"].asFloat();


	return 0;
}


std::string PhuneUser::Serialize(){
	FastWriter writer;
	return writer.write(SerializeToValue());
}

Value PhuneUser::SerializeToValue(){
	Value root;

	root["id"] = id;
	root["registerDate"] = registerDate;
	root["avatar"] = avatar;

	switch (type){
	case REGISTERED:
		root["type"] = "REGISTERED";
		break;
	default:
	case UNREGISTERED:
		root["type"] = "UNREGISTERED";
		break;
	}

	root["credits"] = credits;

	switch (gender)
	{
	case MALE:
		root["gender"] = "MALE";
		break;
	case FEMALE:
		root["gender"] = "FEMALE";
		break;
	case UNKNOWN:
	default:
		root["gender"] = "UNKNOWN";
		break;
	}

	root["name"] = name;
	root["nickname"] = nickname;
	root["xmppPassword"] = xmppPassword;
	root["xmppJID"] = xmppJID;
	root["progress"] = progress;

	return root;
}
