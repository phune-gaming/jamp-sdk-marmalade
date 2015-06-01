#pragma once
#ifndef JAMP_NOTE_PERFORMANCE
#define JAMP_NOTE_PERFORMANCE
#include "../json_object.h"
#include "jamp_classification_type.h"

#include "json/json.h"
#include <string>

using namespace Json;

class JampNotePerformance :
	public JsonObject
{
public:
	virtual std::string Serialize() override;
	virtual Value SerializeToValue() override;
	virtual int32 Deserialize(std::string) override;
	virtual int32 DeserializeFromValue(Value src) override;

public:
	//the note code
	std::string noteCode;
	//note entry error
	int64 entryError;
	//note exit error
	int64 exitError;
	//duration of the note
	int64 noteTime;
	//the hand the note should be played
	bool voice;
	//the classification of the note
	ClassificationType classification;
};
#endif // !JAMP_NOTE_PERFORMANCE
