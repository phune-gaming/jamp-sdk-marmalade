#pragma once
#ifndef JAMP_CELL_PERFORMANCE
#define JAMP_CELL_PERFORMANCE
#include "../json_object.h"
#include "../json_list_object.h"
#include "jamp_classification_type.h"
#include "jamp_note_performance.h"

#include "json/json.h"
#include <string>

using namespace Json;


class JampCellPerformance :
	public JsonObject
{
public:
	virtual std::string Serialize() override;
	virtual Value SerializeToValue() override;
	virtual int32 Deserialize(std::string) override;
	virtual int32 DeserializeFromValue(Value src) override;
public:
	//The id of the cell
	int64 cellId;
	//classification of the cell
	ClassificationType classification;
	//notes performance
	JsonListObject<JampNotePerformance> notesPerformance;
	//Time stamp of the server (automatic)
	int64 timeStamp;
};
#endif // !JAMP_CELL_PERFORMANCE
