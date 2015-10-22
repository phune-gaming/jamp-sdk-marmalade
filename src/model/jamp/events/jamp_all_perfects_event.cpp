#include "jamp_all_perfects_event.h"


AllPerfectsEvent::AllPerfectsEvent(int gameId, std::string level){
    
    char buffer[50];
    sprintf(buffer, "ALL_PERFECTS_%d", gameId);
    type = std::string(buffer);
    
    data = level;
}