#pragma once
#ifndef JAMP_ALL_PERFECTS_EVENT
#define JAMP_ALL_PERFECTS_EVENT

#include "json/json.h"
#include "../../phune/phune_game_triggered_event.h"

#include <string>
#include <sstream>

using namespace Json;

class AllPerfectsEvent :
	public GameTriggerdEvent
{
public:
    AllPerfectsEvent(int gameId, std::string level);
};

#endif // !JAMP_ALL_PERFECTS_EVENT

