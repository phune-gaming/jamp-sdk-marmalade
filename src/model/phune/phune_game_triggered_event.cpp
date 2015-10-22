#include "phune_game_triggered_event.h"


int32 GameTriggerdEvent::Deserialize(std::string jsonSrc)
{
	if (jsonSrc.length() == 0)
		return 0;

	Reader reader;
	Value value;

	reader.parse(jsonSrc, value, false);

	return DeserializeFromValue(value);
}

int32 GameTriggerdEvent::DeserializeFromValue(Value src){
	if (!src["playerId"].empty())
		playerId = src["playerId"].asUInt64();

	if (!src["type"].empty())
        type = src["type"].asString();

	if (!src["data"].empty()){
        data = src["data"].asString();
    }
	
    return 0;
}


std::string GameTriggerdEvent::Serialize(){
	FastWriter writer;
	return writer.write(SerializeToValue());
}

Value GameTriggerdEvent::SerializeToValue(){
    Value root;
    
    
	root["playerId"] = playerId;
	root["type"] = type;
	root["data"] = data;
	
    return root;
}
