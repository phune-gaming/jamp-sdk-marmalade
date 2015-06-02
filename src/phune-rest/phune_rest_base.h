#pragma once
#ifndef PHUNE_REST_BASE
#define PHUNE_REST_BASE

#include <IwHTTP.h>
#include <s3e.h>
#include <IwDebug.h>
#include <string>
#include <queue>
#include <deque>
#include <ctime>
#include <stdlib.h>
#include "s3eWebView.h"


#include "../model/phune/phune_preference.h"
#include "../model/phune/phune_preference_base64.h"
#include "../model/base64.h"
#include "../model/phune/phune_user.h"
#include "../model/json_object.h"
#include "../model/json_list_object.h"
#include "../model/phune/phune_player.h"
#include "../model/jamp/jamp_score.h"
#include "../model/phune/phune_match.h"

//timeout 30 seconds
#define REQUEST_TIMEOUT 30000

//define base url
#define URL_SCHEMA "http"
#define URL_HOST "dev.phune.cadsh.com"
#define URL_PORT "80"
#define URL_CONTEXT "/phune/r"

#define LOGIN_RESOURCE "/me/login/FACEBOOK"
#define LOGIN_REDIRECT "/socialgateway/success"



/*
The request status
*/
enum RequestStatus
{
	READY,
	ONGOING_REQUEST,
	REQUEST_ERROR,
	INVALID_RESPONSE_OBJECT
};

/*
Type of response. Used to parse object.
*/
enum ResponseObjectType
{
	PHUNE_TIMESTAMP,
	PHUNE_USER_OBJECT,
	PHUNE_MATCH_OBJECT,
	PHUNE_PREFERENCE_OBJECT,
	PHUNE_PREFERENCE_JSON,
	PHUNE_PREFERENCE_JSON_LIST,
	PHUNE_STRING,
	PHUNE_REDIRECT_LOGIN,
	NONE
};

enum WebViewRequestStatus
{
	LOADING,
	HIDE_VIEW,
	STOP_VIEW,
	FINISHED,
};

/*
Class to represent a request error.
*/
class RequestError
{
public:
	RequestError(RequestStatus type, uint32 httpCode);
	~RequestError();
	RequestStatus type;
	uint32 httpCode;
};

struct pendingRequest {
	std::string resource;
	CIwHTTP::SendType sendType;
	ResponseObjectType responseObjectType;
	std::string body;
	s3eCallback onResult;
	s3eCallback onError;
};

static std::deque<pendingRequest> _pendingRequests;
inline std::deque<pendingRequest>& pendingRequests() {
	return _pendingRequests;
}

/*
Class to represent a request
*/
class RequestData
{
public:
	RequestData(){};
	RequestData(const char *URI, CIwHTTP *http_object, CIwHTTP::SendType sendType, s3eCallback onResult, ResponseObjectType responseObjectType, const char *body);
	~RequestData();
	static int32 GotHeaders(void*, void*);
	static int32 GotData(void*, void*);
public:
	CIwHTTP *http_object;
	char* result;
	uint32 len;
	s3eCallback onResult;
	RequestStatus requestStatus;
	ResponseObjectType responseObjectType;
	void* base;
	static const char* getUri(const char *resource);

};

class PhuneRestBase
{
public:
	PhuneRestBase();
	~PhuneRestBase();

	/*
	Store a value in server for the user in a game context (Json List). The value as a key that should be used to get/delete the data

	@param gameId The game id.
	@param key The key name.
	@param jsonObject An object of type json. It will be deserialized in json
	@param onResult The callback in case of success.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	template<class T>
	int32 StoreGameDataJsonList(const char *gameId, const char *key, JsonListObject<T> jsonListObject, s3eCallback onResult, s3eCallback onError){
		char resource[200];
		memset(resource, 0, sizeof(resource));
		sprintf(resource, "/jamp/me/preferences/%s?gameId=%s", key, gameId);


		if (!pendingRequests().empty() || onGoingRequest && onGoingRequest->requestStatus == ONGOING_REQUEST)
		{
			pendingRequest pr;
			
			pr.body = jsonListObject.Serialize();
			pr.onError = onError;
			pr.onResult = onResult;
			pr.resource = std::string(resource);
			pr.responseObjectType = NONE;
			pr.sendType = CIwHTTP::PUT;

			pendingRequests().push_back(pr);

			return 0;
		}

		if (onGoingRequest && onGoingRequest->requestStatus == ONGOING_REQUEST)
		{
			onError(new RequestError(ONGOING_REQUEST, 0), NULL);
			return 0;
		}

		if (onGoingRequest && onGoingRequest->requestStatus == READY){
			delete onGoingRequest;
			onGoingRequest = NULL;
		}

		_onResult = onResult;
		_onError = onError;

		PhuneRestBase::onGoingRequest = new RequestData(resource, http_object, CIwHTTP::PUT, GotResult, NONE, jsonListObject.Serialize().c_str());
		PhuneRestBase::onGoingRequest->base = this;

		return 0;
	}

	int32 GetGameDataJsonList(const char *gameId, const char *key, s3eCallback onResult, s3eCallback onError);

	int64 getCurrentTime();
protected:

	int32 _GetMe(s3eCallback onResult, s3eCallback onError);

	
	/*
	Store a value in server for the user in a game context (Base 64). The value as a key that should be used to get/delete the data

	@param gameId The game id.
	@param key The key name.
	@param bytes An array of bytes. The values will be encoded to base64
	@param onResult The callback in case of success.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 StoreGameDataBase64(const char *gameId, const char *key, unsigned char const* bytes, s3eCallback onResult, s3eCallback onError);

	/*
	Store a value in server for the user in a game context (Json). The value as a key that should be used to get/delete the data

	@param gameId The game id.
	@param key The key name.
	@param jsonObject An object of type json. It will be deserialized in json
	@param onResult The callback in case of success.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 StoreGameDataJson(const char *gameId, const char *key, const char* jsonObject, s3eCallback onResult, s3eCallback onError, bool append = false);

	

	/*
	Store a value in server for the user in a game context (Base 64). The value as a key that should be used to get/delete the data

	@param gameId The game id.
	@param key The key name.
	@param onResult The callback in case of success.  It returns in the first paramenter an PhunePreference object.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 GetGameDataBase64(const char *gameId, const char *key, s3eCallback onResult, s3eCallback onError);

	/*
	Store a value in server for the user in a game context (Json). The value as a key that should be used to get/delete the data

	@param gameId The game id.
	@param key The key name.
	@param onResult The callback in case of success.  It returns in the first paramenter an JsonObject object.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 GetGameDataJson(const char *gameId, const char *key, s3eCallback onResult, s3eCallback onError);

	/*
	Delete a value in server for the user in a game context. The value as a key that should be used to get/delete the data

	@param gameId The game id.
	@param key The key name.
	@param onResult The callback in case of success.
	@param onError The callback in case of error. It returns in the first paramenter an RequestError object.
	*/
	int32 DeleteGameData(const char *gameId, const char *key, s3eCallback onResult, s3eCallback onError);

	int32 _Init(s3eCallback onResult, s3eCallback onError);

	int32 _Login(s3eWebView* g_WebView, s3eCallback onResult, s3eCallback onError);

	int32 _StartMatch(const char *gameId, s3eCallback onResult, s3eCallback onError);

	int32 _EndMatch(int64 matchId, PhunePlayer player, s3eCallback onResult, s3eCallback onError);
public:
	

	/*
	Callback in case of success.
	*/
	static s3eCallback _onResult;
	
	/*
	Callback in case of error.
	*/
	static s3eCallback _onError;

	int32 removeOngoingRequest();

protected:
	/*
	Details of the on going request
	*/
	RequestData *onGoingRequest;
	/*
	The http object to communicato to the server
	*/
	CIwHTTP *http_object;
	/*
	Used to get the result
	*/
	static int32 GotResult(void*, void*);

	static int32 _onHideWebView(s3eWebView *instance, void *systemData, void *userData);

private:

	

};


static void* _objectToappend;
inline static void*& objectToAppend(){
	return _objectToappend;
}


static int64 lib_time;
static int64 server_time;



//define the template functions
//#include "phune_rest_base.tpp"

#endif // !PHUNE_REST_BASE

