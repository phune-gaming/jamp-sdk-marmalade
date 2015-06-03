#include "phune_rest.h"


PhuneRest::PhuneRest()
{
}

PhuneRest::~PhuneRest()
{
	
}

int32 PhuneRest::Init(s3eCallback onResult, s3eCallback onError, void *userData){
	return PhuneRestBase::_Init(onResult, onError, userData);
}

int32 PhuneRest::Login(s3eWebView* g_WebView, s3eCallback onResult, s3eCallback onError, void *userData){
	return PhuneRestBase::_Login(g_WebView, onResult, onError, userData);
}

int32 PhuneRest::GetMe(s3eCallback onResult, s3eCallback onError, void *userData){
	return _GetMe(onResult, onError, userData);
}

int32 PhuneRest::StoreGameDataRaw(const char *gameId, const char *key, unsigned char const* bytes, s3eCallback onResult, s3eCallback onError, void *userData){
	return StoreGameDataBase64(gameId, key, bytes, onResult, onError, userData);
}

int32 PhuneRest::GetGameDataBaseRaw(const char *gameId, const char *key, s3eCallback onResult, s3eCallback onError, void *userData){
	return GetGameDataBase64(gameId, key, onResult, onError, userData);
}

int32 PhuneRest::DeleteGameData(const char *gameId, const char *key, s3eCallback onResult, s3eCallback onError, void *userData){
	return PhuneRestBase::DeleteGameData(gameId, key, onResult, onError, userData);
}

int32 PhuneRest::StartMatch(const char *gameId, s3eCallback onResult, s3eCallback onError, void *userData){
	return PhuneRestBase::_StartMatch(gameId, onResult, onError, userData);
}

int32 PhuneRest::EndMatch(int64 matchId, PhunePlayer player, s3eCallback onResult, s3eCallback onError, void *userData){
	return PhuneRestBase::_EndMatch(matchId, player, onResult, onError, userData);
}
