#include "s3e.h"
#include "IwDebug.h"
#include "phune-rest/jamp/phune_rest_jamp.h"
#include <string>
#include <clocale>
#include "s3eWebView.h"
#include "IwUtil.h"

static PhuneRestJamp *pr;
static std::string outStr;
static s3eWebView* g_WebView;

const char* gameId = "1000";

static int32 onError(void *data, void*){
	RequestError *requestError = static_cast<RequestError*>(data);
	if (requestError)
	{
		char aux[100];
		memset(aux, 0, sizeof(aux));
		sprintf(aux, "`x000000[%lld]ERROR:%i (htttpcode:%i)", pr->getCurrentTime(), requestError->type, requestError->httpCode);
		
		outStr.append("\n");
		outStr.append(aux);
	}
	return 0;
}

static int32 onDeleteData(void *data, void*){
	char aux[100];
	sprintf(aux, "`x000000[%lld]Deleted stored values in server!!", pr->getCurrentTime());

	outStr.append("\n");
	outStr.append(aux);

	return 0;
}

static int32 onGetData(void *data, void*)
{
	PhunePreferenceBase64 *phunePreferenceBase64 = static_cast<PhunePreferenceBase64*>(data);
	if (phunePreferenceBase64)
	{
		char aux[200];
		memset(aux, 0, sizeof(aux));
		sprintf(aux, "`x000000[%lld]Received data\nCODED BASE 64:\n%s\nDECODED BASE 64:\n%s", pr->getCurrentTime(), phunePreferenceBase64->valueEncoded.c_str(), phunePreferenceBase64->value.c_str());
		
		outStr.append("\n");
		outStr.append(aux);

		pr->DeleteGameData(gameId, "NOME_DA_KEY", onDeleteData, onError);
	}

	return 0;
}

static int32 onStoreData(void *data, void*){
	char aux[100];
	sprintf(aux, "`x000000[%lld]Stored values in server!!", pr->getCurrentTime());
	
	outStr.append("\n");
	outStr.append(aux);

	pr->GetGameDataBaseRaw(gameId, "NOME_DA_KEY", onGetData, onError);
	return 0;
}

static int32 onGetMeFromStore(void *data, void*){

	PhuneUser *phuneUser = new PhuneUser();
	PhunePreference *phunePreference = static_cast<PhunePreference*>(data);
	phuneUser->Deserialize(phunePreference->value);
	IwTrace(PHUNE, ("Received json code %s", phunePreference->value.c_str()));
	if (phuneUser)
	{
		char aux[100];
		sprintf(aux, "`x000000[%lld]I am %s from store", pr->getCurrentTime(), phuneUser->nickname.c_str());

		outStr.append("\n");
		outStr.append(aux);
	}

	pr->StoreGameDataRaw(gameId, "NOME_DA_KEY", reinterpret_cast<const unsigned char*>("dados quaisqueres que serão convertidos para base 64"), onStoreData, onError);
	return 0;
}

static int32 onStoreMe(void *data, void*){
	char aux[100];
	sprintf(aux, "`x000000[%lld]Stored me in server!!", pr->getCurrentTime());

	outStr.append("\n");
	outStr.append(aux);

	//pr->GetMeFromStore("10", onGetMeFromStore, onError);
	return 0;
}

static int32 onStoreListChange(void *data, void*){
	char aux[100];
	sprintf(aux, "`x000000[%lld]Stored list changed in server!!", pr->getCurrentTime());


	outStr.append("\n");
	outStr.append(aux);

	//pr->GetMeFromStore("10", onGetMeFromStore, onError);
	//pr->GetGameDataJsonList("10", "KEY_LISTA", onGetJsonList, onError);

	return 0;
}

static int32 onGetJsonList(void *data, void*){

	PhunePreference *phunePreference = static_cast<PhunePreference*>(data);
	JsonListObject<PhuneUser> listOfusers;
	PhuneUser phuneUser;

	listOfusers.Deserialize(phunePreference->value);
	phuneUser = listOfusers.top();

	
	char aux[100];
	sprintf(aux, "`x000000[%lld]I am %s from list",pr->getCurrentTime(), phuneUser.nickname.c_str());

	outStr.append("\n");
	outStr.append(aux);

	listOfusers.elements[1].name = std::string("José Manuel Antunes");

	pr->StoreGameDataJsonList(gameId, "KEY_LISTA", listOfusers, onStoreListChange, onError);

	return 0;
	
}

static int32 onStoreList(void *data, void*){
	char aux[100];
	sprintf(aux, "`x000000[%lld]Stored list in server!!", pr->getCurrentTime());

	outStr.append("\n");
	outStr.append(aux);

	//pr->GetMeFromStore("10", onGetMeFromStore, onError);
	pr->GetGameDataJsonList(gameId, "KEY_LISTA", onGetJsonList, onError);
	
	return 0;
}

static int32 onResultPhuneUser(void *data, void*){
	PhuneUser *phuneUser = static_cast<PhuneUser*>(data);
	if (phuneUser)
	{
		char aux[100];
		sprintf(aux, "`x000000[%lld]I am %s",pr->getCurrentTime(), phuneUser->nickname.c_str());

		outStr.append("\n");
		outStr.append(aux);
	}

	JsonListObject<PhuneUser> listOfusers;

	//listOfusers->outraCoisa();
	listOfusers.pushElement(*phuneUser);
	listOfusers.pushElement(*phuneUser);
	listOfusers.pushElement(*phuneUser);
	listOfusers.pushElement(*phuneUser);
	listOfusers.pushElement(*phuneUser);
	listOfusers.pushElement(*phuneUser);
	listOfusers.pushElement(*phuneUser);

	

	pr->StoreGameDataJsonList(gameId, "KEY_LISTA", listOfusers, onStoreList, onError);

	return 0;
}

static int32 onEndMatch(void *data, void*){
	char aux[100];
	sprintf(aux, "`x000000[%lld]End match success!!", pr->getCurrentTime());

	outStr.append("\n");
	outStr.append(aux);

	pr->GetMe(onResultPhuneUser, onError);

	return 0;
}

static int32 onMatchId(void *data, void*){
	//char *matchId = (char *)(data);
	char aux[100];
	sprintf(aux, "`x000000[%lld]I received a match", pr->getCurrentTime());

	outStr.append("\n");
	outStr.append(aux);

	JampScore score;
	score.score = 20;
	score.timeSpent = 10000;


	JampNotePerformance notePerformance;
	notePerformance.classification = PERFECT;
	notePerformance.entryError = 100;
	notePerformance.exitError = 100;
	notePerformance.noteCode = "B";
	notePerformance.noteTime = 1000;

	JampCellPerformance cellPerformace;
	cellPerformace.cellId = "IDDEUMACELULA";
	cellPerformace.classification = GOOD;
		
	cellPerformace.notesPerformance.pushElement(notePerformance);
	notePerformance.classification = GOOD;
	notePerformance.noteCode = "A";
	cellPerformace.notesPerformance.pushElement(notePerformance);
	notePerformance.classification = BAD;
	notePerformance.noteCode = "C";
	cellPerformace.notesPerformance.pushElement(notePerformance);

	score.cellsPerformance.pushElement(cellPerformace);
	score.cellsPerformance.pushElement(cellPerformace);

	JampPack *pack = new JampPack();
	pack->packId = 2;
	pack->blocked = false;
	pack->starsMax = 32;
	pack->starsWon = 11;

	//pr->EndMatch(matchId, pp, onEndMatch, onError);
	pr->EndMatch(RHYTHM, 1, score, WINNER, onEndMatch, onError);

	//pr->GetMe(onResultPhuneUser, onError);

	
	return 0;
}

static int32 onMe(void *data, void*){
	IwTrace(PHUNE, ("onMe"));
	PhuneUser *phuneUser = static_cast<PhuneUser*>(data);
	if (phuneUser)
	{
		char aux[100];
		sprintf(aux, "`x000000[%lld]I am %s", pr->getCurrentTime(), phuneUser->nickname.c_str());

		outStr.append("\n");
		outStr.append(aux);
	}

	return 0;
}

static int32 onWebView(void *data, void*){
	IW_CALLSTACK("PhuneRestSDK::onWebView");
	IwTrace(PHUNE, ("onWebView"));
	int *result = static_cast<int*>(data);

	char aux[100];
	switch (*result)
	{
	case LOADING:
		IwTrace(PHUNE, ("onWebView LOADING"));
		sprintf(aux, "`x000000[%lld]On WebView LOADING!!", pr->getCurrentTime());
		break;
	case HIDE_VIEW:
		s3eWebViewHide(g_WebView);
		IwTrace(PHUNE, ("onWebView HIDE_VIEW"));
		sprintf(aux, "`x000000[%lld]On WebView HIDE_VIEW!!", pr->getCurrentTime());
		break;
	case STOP_VIEW:
		s3eWebViewDestroy(g_WebView);
		g_WebView = NULL;
		IwTrace(PHUNE, ("onWebView STOP_VIEW"));
		sprintf(aux, "`x000000[%lld]On WebView STOP_VIEW!!", pr->getCurrentTime());
		break;
	case FINISHED:
		IwTrace(PHUNE, ("onWebView FINISHED"));
		sprintf(aux, "`x000000[%lld]On WebView FINISHED!!", pr->getCurrentTime());

		//Calling a new me :) after login
		pr->GetMe(onMe, onError);
		break;
	default:
		IwTrace(PHUNE, ("onWebView default"));
		sprintf(aux, "`x000000[%lld]On WebView DEFAULT!!", pr->getCurrentTime());
		break;
	}




	outStr.append("\n");
	outStr.append(aux);

	return 0;
}

static int32 onMeAfterInit(void *data, void*){
	IwTrace(PHUNE, ("onMeAfterInit"));
	PhuneUser *phuneUser = static_cast<PhuneUser*>(data);
	if (phuneUser)
	{
		char aux[100];
		sprintf(aux, "`x000000[%lld]I am %s", pr->getCurrentTime(), phuneUser->nickname.c_str());

		outStr.append("\n");
		outStr.append(aux);

		//do the login in case of unresgistered
		if (phuneUser->type == UNREGISTERED){

			g_WebView = s3eWebViewCreate();

			s3eWebViewShow(g_WebView, 10, 100, s3eSurfaceGetInt(S3E_SURFACE_WIDTH) - 20, s3eSurfaceGetInt(S3E_SURFACE_HEIGHT) - 110);
			pr->Login(g_WebView, onWebView, onError);
		}
	}

	return 0;
}

static int32 onInit(void *data, void*){
	char aux[100];
	sprintf(aux, "`x000000[%lld]On Init!!", pr->getCurrentTime());

	outStr.append("\n");
	outStr.append(aux);



	//pr->GetMe(onMeAfterInit, onError);
	//pr->Login(g_WebView, onWebView, onError);

	return 0;
}

static int32 onGetScore(void *data, void*){
	PhunePreference *phunePreference = static_cast<PhunePreference*>(data);
	JsonListObject<JampScore> listOfscores;

	listOfscores.Deserialize(phunePreference->value);

	char aux[100];
	unsigned int i = 0;
	while (i < listOfscores.elements.size()){
		sprintf(aux, "`x000000[%lld]I have points %lld in match %d", pr->getCurrentTime(), listOfscores.elements.at(i).score, listOfscores.elements.at(i).matchId);
		outStr.append("\n");
		outStr.append(aux);
		i++;
	}	

	

	return 0;
}



// Main entry point for the application
int main()
{

	std::setlocale(LC_ALL, "en_US.UTF-8");
	//Initialise graphics system(s)
	pr = new PhuneRestJamp();
	PhunePlayer player;
	player.score = 10;
	player.status = WINNER;
	
	//pr->EndMatch("1000", player, onEndMatch, onError);
	pr->Init(onInit, onError);

	

	pr->StartMatch(RHYTHM, onMatchId, onError);
	//pr->GetScoreForMatch("1000", "1", onGetScore, onError);

	//pr->StartMatch(gameId, onMatchId, onError);

	outStr = "NONE";


	// Loop forever, until the user or the OS performs some action to quit the app
	while (!s3eDeviceCheckQuitRequest())
	{
		//Update the input systems
		s3eKeyboardUpdate();
		s3ePointerUpdate();


		s3eSurfaceClear(255, 255, 255);

		// Print a line of debug text to the screen at top left (0,0)
		// Starting the text with the ` (backtick) char followed by 'x' and a hex value
		// determines the colour of the text.
		s3eDebugPrint(0, 20, outStr.c_str(), 0);


		// Flip the surface buffer to screen
		s3eSurfaceShow();

		

		// Sleep for 0ms to allow the OS to process events etc.
		s3eDeviceYield(0);
	}

	if (g_WebView)
	{

		s3eWebViewDestroy(g_WebView);

		g_WebView = NULL;

	}

	//Terminate modules being used
	delete pr;
	// Return
	return 0;
}
