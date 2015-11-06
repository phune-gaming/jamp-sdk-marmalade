#include "jamp_game_progress.h"


int32 JampGameProgress::Deserialize(std::string jsonSrc)
{
	
	if (jsonSrc.length() == 0)
		return 0;

	Reader reader;
	Value value;

	reader.parse(jsonSrc, value, false);

	return DeserializeFromValue(value);
}

int32 JampGameProgress::DeserializeFromValue(Value src){
	if (!src["progress"].empty())
        progress = src["progress"].asFloat();
	
	return 0;
}


std::string JampGameProgress::Serialize(){
	FastWriter writer;
	return writer.write(SerializeToValue());
}

Value JampGameProgress::SerializeToValue(){
	Value root;

    if(progress >= 0)
        root["progress"] = progress;

	return root;
}
