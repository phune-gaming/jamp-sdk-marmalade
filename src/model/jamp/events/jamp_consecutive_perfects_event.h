#pragma once
#ifndef JAMP_CONSECUTIVE_PERFECTS_EVENT
#define JAMP_CONSECUTIVE_PERFECTS_EVENT

#include "json/json.h"
#include "../../phune/phune_game_triggered_event.h"

#include <string>
#include <sstream>

using namespace Json;

class ConsecutivePerfectsEvent :
	public GameTriggerdEvent
{
public:
    ConsecutivePerfectsEvent(int, int);

public:
    int consecutivePerfects;
};

#endif // !JAMP_CONSECUTIVE_PERFECTS_EVENT

