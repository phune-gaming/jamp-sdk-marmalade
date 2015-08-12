#include "json_string.h"


int32 JsonString::Deserialize(std::string jsonSrc)
{
    value = jsonSrc;
    
    return 0;
}

int32 JsonString::DeserializeFromValue(Json::Value src){
    return 0;
}


std::string JsonString::Serialize(){
    FastWriter writer;
    return writer.write(SerializeToValue());
}

Value JsonString::SerializeToValue(){
    Value root;
    
    root = value;
    
    
    return root;
}
