#include "phune_rest_jamp.h"

static s3eCallback onResultPending;
static PhuneMatch *currentMatch = NULL;

static int32 onNullReturn(void *, void*){
	return 0;
}

static int32 onStartMatchReturn(void *data, void* userData){
	currentMatch = static_cast<PhuneMatch*>(data);
	onResultPending(NULL, userData);
	return 0;
}

int32 PhuneRestJamp::Init(s3eCallback onResult, s3eCallback onError, void *userData){
	return PhuneRest::Init(onResult, onError, userData);
}

int32 PhuneRestJamp::Login(s3eWebView* g_WebView, s3eCallback onResult, s3eCallback onError, void *userData){
	return PhuneRest::Login(g_WebView, onResult, onError, userData);
}

int32 PhuneRestJamp::GetMe(s3eCallback onResult, s3eCallback onError, void *userData){
	return PhuneRest::GetMe(onResult, onError, userData);
}

int32 PhuneRestJamp::StartMatch(JampGameId gameId, s3eCallback onResult, s3eCallback onError, void *userData){
	char buffer[50];
	sprintf(buffer, "%d", gameId);
	std::string s = std::string(buffer);
	onResultPending = onResult;
	return PhuneRest::StartMatch(s.c_str(), onStartMatchReturn, onError, userData);
}

int32 PhuneRestJamp::EndMatch(JampGameId gameId, int64 level, JampScore score, PlayerStatus status, s3eCallback onResult, s3eCallback onError, void *userData){
	if (currentMatch == NULL){
		onError(NULL, NULL);
		return 0;
	}
	char buffer[50];
	sprintf(buffer, "%d", gameId);
	std::string s = std::string(buffer);


	char buffer3[50];
	sprintf(buffer3, "%s%d", SCORE_LEVEL_KEY_PREFIX, level);
	std::string s3 = std::string(buffer3);

	
	score.matchId = currentMatch->matchId;

	PhunePlayer player;
	player.score = score.score;
	player.status = status;

	int64 t = getCurrentTime();

	//Send the result of the match
	PhuneRest::EndMatch(score.matchId, player, onNullReturn, onError);

	

	//store the Cell performances
	for (std::vector<JampCellPerformance>::iterator it = score.cellsPerformance.elements.begin(); it != score.cellsPerformance.elements.end(); it++){
		std::string key_cell = std::string(CELL_PERFORMACE_PREFIX);
		key_cell.append(it->cellId);
		it->timeStamp = t;
		PhuneRestBase::StoreGameDataJson(s.c_str(), key_cell.c_str(), it->Serialize().c_str(), onNullReturn, onError, userData, true);
	}

	//store the score for the match
	score.timeStamp = t;
	PhuneRestBase::StoreGameDataJson(s.c_str(), s3.c_str(), score.Serialize().c_str(), onResult, onError, userData, true);
	
	//TODO send the events

	delete currentMatch;
	currentMatch = NULL;

	return 0;
}

int32 PhuneRestJamp::GetHistoricScoreForLevelInGame(JampGameId gameId, const char *level, s3eCallback onResult, s3eCallback onError, void *userData){
	char buffer[50];
	sprintf(buffer, "%d", gameId);
	std::string s = std::string(buffer);

	std::string key_score = std::string(SCORE_LEVEL_KEY_PREFIX);
	key_score.append(level);
	return PhuneRestBase::GetGameDataJsonList(s.c_str(), key_score.c_str(), onResult, onError, userData);
}

int32 PhuneRestJamp::StorePackInfoInGame(JampGameId gameId, JampPack pack, s3eCallback onResult, s3eCallback onError, void *userData){
	char buffer[50];
	sprintf(buffer, "%d", gameId);
	std::string s = std::string(buffer);

	char buffer2[50];
	sprintf(buffer2, "%s%d", PACK_PREFIX, gameId);
	std::string s2 = std::string(buffer2);

	return PhuneRestBase::StoreGameDataJson(s.c_str(), s2.c_str(), pack.Serialize().c_str(), onResult, onError, userData);
}

int32 PhuneRestJamp::GetPackInfoInGame(JampGameId gameId, int64 packid, s3eCallback onResult, s3eCallback onError, void *userData){
	char buffer[50];
	sprintf(buffer, "%d", gameId);
	std::string s = std::string(buffer);

	char buffer2[50];
	sprintf(buffer2, "%s%d", PACK_PREFIX, gameId);
	std::string s2 = std::string(buffer2);

	return PhuneRestBase::GetGameDataJson(s.c_str(), s2.c_str(), onResult, onError, userData);
}

int32 PhuneRestJamp::StorePacksInGame(JampGameId gameId, JsonListObject<JampPack> packs, s3eCallback onResult, s3eCallback onError, void *userData){
	char buffer[50];
	sprintf(buffer, "%d", gameId);
	std::string s = std::string(buffer);

	return PhuneRestBase::StoreGameDataJsonList(s.c_str(), PACKS_KEY, packs, onResult, onError, userData);
}

int32 PhuneRestJamp::GetPacksInGame(JampGameId gameId, s3eCallback onResult, s3eCallback onError, void *userData){
	char buffer[50];
	sprintf(buffer, "%d", gameId);
	std::string s = std::string(buffer);

	return PhuneRestBase::GetGameDataJsonList(s.c_str(), PACKS_KEY, onResult, onError, userData);
}
