#include "jamp_consecutive_perfects_event.h"


ConsecutivePerfectsEvent::ConsecutivePerfectsEvent(int consecutivePerfects, int gameId){
    this->consecutivePerfects = consecutivePerfects;

    std::stringstream ss;
    ss << consecutivePerfects;
    data = ss.str();
    
    char buffer[50];
    sprintf(buffer, "CONSECUTIVE_PERFECTS_%d", gameId);
    type = std::string(buffer);
}