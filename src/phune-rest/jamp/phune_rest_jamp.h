#pragma once
#ifndef PHUNE_REST_JAMP
#define PHUNE_REST_JAMP

#include "../phune_rest.h"
#include "../../model/jamp/jamp_score.h"
#include "../../model/jamp/jamp_pack.h"
#include "../../model/json_list_object.h"
#include "../../model/phune/phune_match.h"

#include <stdlib.h>

#define SCORE_LEVEL_KEY_PREFIX "SCORE_"
#define CELL_PERFORMACE_PREFIX "CELL_"
#define PACK_PREFIX "PACK_"
#define PACKS_KEY "PACKS"

enum JampGameId
{
	PLATFORM = -1,
	RHYTHM = 1000,
	//Conhecimento teclado
	//Esclas e harpejos
	// 5notas
	//1� vista
	REPERTOIRE = 30
};

class PhuneRestJamp : public PhuneRest
{
public:
	/*
	Init the sdk.

	@param onResult The callback in case of success.
	@param onError The callback in case of error.
	*/
	int32 Init(s3eCallback onResult, s3eCallback onError);

	/*
	Do the Login (Facebook)

	@param g_WebView The Web view created by the UI.
	@param onResult The calbacks during the login. It sends a int* in data as WebViewRequestStatus: LOADING->The web page is loading; HIDE_VIEW->It should hide the Web view; STOP_VIEW->It should close/kill the Web view; FINISHED->The Login process is completed. It should ask for "GetMe" to obtain the new user.
	@param onError The callback in case of error.
	*/
	int32 Login(s3eWebView* g_WebView, s3eCallback onResult, s3eCallback onError);
	
	/*
	Get my Phune User or create one. It puts the user in the session.

	@param onResult The callback in case of success. It returns in the first paramenter an PhuneUser object.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 GetMe(s3eCallback onResult, s3eCallback onError);

	/*
	Inform that a match has been started. In return it will be retruned a match id to be used when storing a core of a match
	
	@param gameId The game id.
	@param onResult The callback in case of success. It returns in the first paramenter a match id as a char*.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 StartMatch(JampGameId gameId, s3eCallback onResult, s3eCallback onError);

	/*
	Inform that a match has been completed a to store the performace of the match. It will store the matches performances.

	@param gameId The game id.
	@param level The level of the match
	@param score the object score with the performance of the level
	@param status Inform the server if it is a win (complete the level) or a lose (didn�t complete the level).
	@param onResult The callback in case of success.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	@param pack (Optional) pack information after match (if any changes occurred).
	*/
	int32 EndMatch(JampGameId gameId, const char *level, JampScore score, PlayerStatus status, s3eCallback onResult, s3eCallback onError);

	/*
	Get the historical data for level in Game.

	@param gameId The game id.
	@param level The level of the match
	@param onResult The callback in case of success. Return a list of JampScore 
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 GetHistoricScoreForLevelInGame(JampGameId gameId, const char *level, s3eCallback onResult, s3eCallback onError);

	/*
	Stores a pack in Game.

	@param gameId The game id.
	@param pack The pack,
	@param onResult The callback in case of success.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 StorePackInfoInGame(JampGameId gameId, JampPack pack, s3eCallback onResult, s3eCallback onError);

	/*
	Get pack info in Game.

	@param gameId The game id.
	@param packId The pack id
	@param onResult The callback in case of success. Return a JampPack.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 GetPackInfoInGame(JampGameId gameId, const char *packId, s3eCallback onResult, s3eCallback onError);

	/*
	Stores the list of packs in Game.

	@param gameId The game id.
	@param pack The List of packs,
	@param onResult The callback in case of success.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 StorePacksInGame(JampGameId gameId, JsonListObject<JampPack> packs, s3eCallback onResult, s3eCallback onError);

	/*
	Get list of packs in Game.

	@param gameId The game id.
	@param packId The pack id
	@param onResult The callback in case of success. Return a JsonListObject<JampPack> packs.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 GetPacksInGame(JampGameId gameId, s3eCallback onResult, s3eCallback onError);


};

//s3eCallback PhuneRestBase::_onResult;
//s3eCallback PhuneRestBase::_onError;

#endif // !PHUNE_REST_JAMP