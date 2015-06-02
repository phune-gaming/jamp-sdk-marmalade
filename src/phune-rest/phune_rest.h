#pragma once
#ifndef PHUNE_REST
#define PHUNE_REST

#include "s3eWebView.h"

#include "phune_rest_base.h"
#include "../model/phune/phune_user.h"
#include "../model/jamp/jamp_score.h"
#include "../model/json_list_object.h"

class PhuneRest : public PhuneRestBase
{
public:
	PhuneRest();
	~PhuneRest();

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
	Store a value in server for the user in a game context as a raw value. The value as a key that should be used to get/delete the data

	@param gameId The game id.
	@param key The key name.
	@param bytes An array of bytes.
	@param onResult The callback in case of success.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 StoreGameDataRaw(const char *gameId, const char *key, unsigned char const* bytes, s3eCallback onResult, s3eCallback onError);

	/*
	Store a value in server for the user in a game context as a raw value. The value as a key that should be used to get/delete the data

	@param gameId The game id.
	@param key The key name.
	@param onResult The callback in case of success.  It returns in the first paramenter an PhunePreference object.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 GetGameDataBaseRaw(const char *gameId, const char *key, s3eCallback onResult, s3eCallback onError);

	/*
	Delete a value in server for the user in a game context. The value as a key that should be used to get/delete the data

	@param gameId The game id.
	@param key The key name.
	@param onResult The callback in case of success.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 DeleteGameData(const char *gameId, const char *key, s3eCallback onResult, s3eCallback onError);

	/*
	Inform that a match has been started. In return it will be retruned a match id to be used when storing a core of a match
	
	@param gameId The game id.
	@param onResult The callback in case of success. It returns in the first paramenter a match id as a char*.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 StartMatch(const char *gameId, s3eCallback onResult, s3eCallback onError);

	/*
	Inform the server the match is end.

	@param matchId The match id. Obtained in start match.
	@param player The player of the match with the score and result.
	@param onResult The callback in case of success. It returns in the first paramenter a match id as a char*.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 EndMatch(int64 matchId, PhunePlayer player, s3eCallback onResult, s3eCallback onError);

};

//s3eCallback PhuneRestBase::_onResult;
//s3eCallback PhuneRestBase::_onError;

#endif // !PHUNE_REST