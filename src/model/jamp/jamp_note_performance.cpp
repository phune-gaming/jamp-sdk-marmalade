#include "jamp_note_performance.h"


int32 JampNotePerformance::Deserialize(std::string jsonSrc)
{
	if (jsonSrc.length() == 0)
		return 0;

	Reader reader;
	Value value;

	reader.parse(jsonSrc, value, false);

	return DeserializeFromValue(value);
}

int32 JampNotePerformance::DeserializeFromValue(Value src){

	if (!src["noteCode"].empty())
		noteCode = src["noteCode"].asString();

	if (!src["entryError"].empty())
		entryError = src["entryError"].asUInt64();

	if (!src["exitError"].empty())
		exitError = src["exitError"].asUInt64();

	if (!src["noteTime"].empty())
		noteTime = src["noteTime"].asUInt64();

	if (!src["voice"].empty())
		voice = src["voice"].asBool();

	if (!src["classification"].empty()){
		std::string classificationStr = src["classification"].asString();
		if (classificationStr.compare("PERFECT") == 0)
			classification = PERFECT;
		else if (classificationStr.compare("GOOD") == 0)
			classification = GOOD;
		else if (classificationStr.compare("BAD") == 0)
			classification = BAD;
		else if (classificationStr.compare("MISS") == 0)
			classification = MISS;
	}

	return 0;
}


std::string JampNotePerformance::Serialize(){
	FastWriter writer;
	return writer.write(SerializeToValue());
}

Value JampNotePerformance::SerializeToValue(){
	Value root;

	root["noteCode"] = noteCode;
	root["entryError"] = entryError;
	root["exitError"] = exitError;
	root["noteTime"] = noteTime;
	root["voice"] = voice;

	switch (classification){
		case PERFECT:
			root["classification"] = "PERFECT";
			break;
		case GOOD:
			root["classification"] = "GOOD";
			break;
		case BAD:
			root["classification"] = "BAD";
			break;
		case MISS:
			root["classification"] = "MISS";
			break;
	}


	return root;
}

