#include "phune_rest_base.h"

using namespace std;

//define
s3eCallback PhuneRestBase::_onResult;
s3eCallback PhuneRestBase::_onError;

PhuneRestBase::PhuneRestBase()
{

	http_object = new CIwHTTP;

	//http_object->SetRequestHeader("Cache-Control", "max-age=0");
	http_object->SetRequestHeader("Accept", "application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,image/png,*/*;q=0.5");
	//http_object->SetRequestHeader("Accept-Encoding", "gzip,deflate,sdch");
	http_object->SetRequestHeader("Accept-Language", "en-GB");
	http_object->SetRequestHeader("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.3");
	http_object->SetRequestHeader("Content-Type", "application/json; charset=UTF-8");
	


	s3eFile*  cookie_file = s3eFileOpen("phune_cookie.cookie", "r");
	if (cookie_file){
		int32 fileSize = s3eFileGetSize(cookie_file);
		char pFileData[500];
		std::memset(pFileData, 0, sizeof(pFileData));

		if ((int32)s3eFileRead(pFileData, 1, fileSize, cookie_file) == fileSize)
		{
			IwTrace(PHUNE, ("Using Cookie from previous session %s", pFileData));

			//deprecated to remove after server supports the authorization header
			std::string cookie_prefix = "JSESSIONID=";
			cookie_prefix.append(pFileData);
			http_object->SetRequestHeader("Cookie", cookie_prefix.c_str());

			std::string authorization_prefix = "Bearer ";
			authorization_prefix.append(pFileData);
			http_object->SetRequestHeader("Authorization", authorization_prefix.c_str());

		}
		else
		{
			IwTrace(PHUNE, ("Could not read from Cookie file"));
			//s3eFree(pFileData);
		}

		s3eFileClose(cookie_file);

	}
	else{
		IwTrace(PHUNE, ("Could not find Cookie file"));
	}

}


PhuneRestBase::~PhuneRestBase()
{
	if (onGoingRequest != NULL)
	{
		delete onGoingRequest;
		onGoingRequest = NULL;
	}

	if (http_object)
	{
		delete http_object;
		http_object = NULL;
	}
}

int64 PhuneRestBase::getCurrentTime(){
	return server_time + (clock() - lib_time);
	//return 0;
}

int32 PhuneRestBase::_Init(s3eCallback onResult, s3eCallback onError, void *userData){

	char resource[200];
	std::memset(resource, 0, sizeof(resource));
	sprintf(resource, "/jamp/utils/time");

	_onResult = onResult;
	_onError = onError;


	onGoingRequest = new RequestData(resource, http_object, CIwHTTP::GET, GotResult, PHUNE_TIMESTAMP, NULL, userData);

	return 0;
}

int32 PhuneRestBase::_Login(s3eWebView* g_WebView, s3eCallback onResult, s3eCallback onError, void *userData){
	std::string  uri;

	uri.append(URL_SCHEMA);
	//uri.append("s");
	uri.append("://");
	uri.append(URL_HOST);
	//uri->append(":");
	//uri->append(URL_PORT);
	uri.append(URL_CONTEXT);
	uri.append(LOGIN_RESOURCE);
	//uri.append("?");
	//uri.append(LOGIN_REDIRECT);

	if (onGoingRequest && onGoingRequest->requestStatus == READY){
		IwTrace(PHUNE, ("PhuneRestBase::_Login Delete ongoing data"));
		delete onGoingRequest;
		onGoingRequest = NULL;
	}

	_onResult = onResult;
	_onError = onError;
	//pendingUserData = userData;

	
	//PhuneRestBase::onWebView = _onWebView;
	s3eResult result1 = s3eWebViewRegister(S3E_WEBVIEW_FINISHED_LOADING, _onHideWebView, this, g_WebView);
	
	switch (result1)
	{
	case S3E_RESULT_SUCCESS:
		IwTrace(PHUNE, ("SUCCESS ON REGISTER"));
		break;
	case S3E_RESULT_ERROR:
	default:
		IwTrace(PHUNE, ("ERROR ON REGISTER"));
		onError(NULL, NULL);
		return 0;
	}

	s3eResult result2 = s3eWebViewNavigate(g_WebView, uri.c_str());

	int tmp = LOADING;
	switch (result2)
	{
	case S3E_RESULT_SUCCESS:
		onResult(&tmp, NULL);
		break;
	case S3E_RESULT_ERROR:
	default:
		onError(NULL, NULL);
		return 0;
	}

	

	return 0;
}

int32 PhuneRestBase::_GetMe(s3eCallback onResult, s3eCallback onError, void *userData){

	if (!pendingRequests().empty() || onGoingRequest && onGoingRequest->requestStatus == ONGOING_REQUEST)
	{
		pendingRequest pr;
		pr.onError = onError;
		pr.onResult = onResult;
		pr.resource = std::string("/me");
		pr.responseObjectType = PHUNE_USER_OBJECT;
		pr.sendType = CIwHTTP::GET;
		pr.userData = userData;
		pendingRequests().push_back(pr);

		return 0;
	}

	if (onGoingRequest && onGoingRequest->requestStatus == READY){
		delete onGoingRequest;
		onGoingRequest = NULL;
	}

	_onResult = onResult;
	_onError = onError;
	
	onGoingRequest = new RequestData("/me", http_object, CIwHTTP::GET, GotResult, PHUNE_USER_OBJECT, NULL, userData);


	return 0;
}

int32 PhuneRestBase::StoreGameDataBase64(const char *gameId, const char *key, unsigned char const *bytes_to_encode, s3eCallback onResult, s3eCallback onError, void *userData)
{
	char resource[200];
	std::memset(resource, 0, sizeof(resource));
	sprintf(resource, "/jamp/me/preferences/%s?gameId=%s", key, gameId);

	if (!pendingRequests().empty() || onGoingRequest && onGoingRequest->requestStatus == ONGOING_REQUEST)
	{
		pendingRequest pr;
		pr.body = base64_encode(bytes_to_encode, strlen((char*)bytes_to_encode));
		pr.onError = onError;
		pr.onResult = onResult;
		pr.resource = std::string(resource);
		pr.responseObjectType = NONE;
		pr.sendType = CIwHTTP::PUT;
		pr.userData = userData;
		pendingRequests().push_back(pr);

		return 0;
	}

	if (onGoingRequest && onGoingRequest->requestStatus == READY){
		delete onGoingRequest;
		onGoingRequest = NULL;
	}

	_onResult = onResult;
	_onError = onError;


	onGoingRequest = new RequestData(resource, http_object, CIwHTTP::PUT, GotResult, NONE, base64_encode(bytes_to_encode, strlen((char*)bytes_to_encode)).c_str(), userData);

	return 0;
}

int32 PhuneRestBase::StoreGameDataJson(const char *gameId, const char *key, const char* jsonObject, s3eCallback onResult, s3eCallback onError, void *userData, bool append){

	char resource[200];
	std::memset(resource, 0, sizeof(resource));
	
	if (append){
		sprintf(resource, "/jamp/me/preferences/%s?gameId=%s&append=true", key, gameId);
	}
	else
	{
		sprintf(resource, "/jamp/me/preferences/%s?gameId=%s", key, gameId);
	}

	if (!pendingRequests().empty() || onGoingRequest && onGoingRequest->requestStatus == ONGOING_REQUEST)
	{
		pendingRequest pr;
		pr.body = jsonObject;
		pr.onError = onError;
		pr.onResult = onResult;
		pr.resource = std::string(resource);
		pr.responseObjectType = NONE;
		pr.sendType = CIwHTTP::PUT;
		pr.userData = userData;
		pendingRequests().push_back(pr);

		return 0;
	}

	if (onGoingRequest && onGoingRequest->requestStatus == READY){
		delete onGoingRequest;
		onGoingRequest = NULL;
	}

	_onResult = onResult;
	_onError = onError;


	onGoingRequest = new RequestData(resource, http_object, CIwHTTP::PUT, GotResult, NONE, jsonObject, userData);

	return 0;
}

int32 PhuneRestBase::GetGameDataBase64(const char *gameId, const char *key, s3eCallback onResult, s3eCallback onError, void *userData)
{
	char resource[200];
	std::memset(resource, 0, sizeof(resource));
	sprintf(resource, "/jamp/me/preferences/%s?gameId=%s", key, gameId);

	if (!pendingRequests().empty() || onGoingRequest && onGoingRequest->requestStatus == ONGOING_REQUEST)
	{
		pendingRequest pr;
		pr.onError = onError;
		pr.onResult = onResult;
		pr.resource = std::string(resource);
		pr.responseObjectType = PHUNE_PREFERENCE_OBJECT;
		pr.sendType = CIwHTTP::GET;
		pr.userData = userData;
		pendingRequests().push_back(pr);

		return 0;
	}

	if (onGoingRequest && onGoingRequest->requestStatus == READY){
		delete onGoingRequest;
		onGoingRequest = NULL;
	}

	_onResult = onResult;
	_onError = onError;


	onGoingRequest = new RequestData(resource, http_object, CIwHTTP::GET, GotResult, PHUNE_PREFERENCE_OBJECT, NULL, userData);

	return 0;
}

int32 PhuneRestBase::GetGameDataJson(const char *gameId, const char *key, s3eCallback onResult, s3eCallback onError, void *userData){

	char resource[200];
	std::memset(resource, 0, sizeof(resource));
	sprintf(resource, "/jamp/me/preferences/%s?gameId=%s", key, gameId);

	if (!pendingRequests().empty() || onGoingRequest && onGoingRequest->requestStatus == ONGOING_REQUEST)
	{
		pendingRequest pr;
		pr.onError = onError;
		pr.onResult = onResult;
		pr.resource = std::string(resource);
		pr.responseObjectType = PHUNE_PREFERENCE_JSON;
		pr.sendType = CIwHTTP::GET;
		pr.userData = userData;
		pendingRequests().push_back(pr);

		return 0;
	}

	if (onGoingRequest && onGoingRequest->requestStatus == READY){
		delete onGoingRequest;
		onGoingRequest = NULL;
	}

	_onResult = onResult;
	_onError = onError;


	onGoingRequest = new RequestData(resource, http_object, CIwHTTP::GET, GotResult, PHUNE_PREFERENCE_JSON, NULL, userData);

	return 0;
}

int32 PhuneRestBase::GetGameDataJsonList(const char *gameId, const char *key, s3eCallback onResult, s3eCallback onError, void *userData){

	char resource[200];
	std::memset(resource, 0, sizeof(resource));
	sprintf(resource, "/jamp/me/preferences/%s?gameId=%s", key, gameId);

	if (!pendingRequests().empty() || onGoingRequest && onGoingRequest->requestStatus == ONGOING_REQUEST)
	{
		pendingRequest pr;
		pr.onError = onError;
		pr.onResult = onResult;
		pr.resource = std::string(resource);
		pr.responseObjectType = PHUNE_PREFERENCE_JSON_LIST;
		pr.sendType = CIwHTTP::GET;
		pr.userData = userData;
		pendingRequests().push_back(pr);

		return 0;
	}

	if (onGoingRequest && onGoingRequest->requestStatus == READY){
		delete onGoingRequest;
		onGoingRequest = NULL;
	}

	_onResult = onResult;
	_onError = onError;

	onGoingRequest = new RequestData(resource, http_object, CIwHTTP::GET, GotResult, PHUNE_PREFERENCE_JSON_LIST, NULL, userData);

	return 0;
}

int32 PhuneRestBase::DeleteGameData(const char *gameId, const char *key, s3eCallback onResult, s3eCallback onError, void *userData)
{
	char resource[200];
	std::memset(resource, 0, sizeof(resource));
	sprintf(resource, "/jamp/me/preferences/%s?gameId=%s", key, gameId);

	if (!pendingRequests().empty() || onGoingRequest && onGoingRequest->requestStatus == ONGOING_REQUEST)
	{
		pendingRequest pr;
		pr.onError = onError;
		pr.onResult = onResult;
		pr.resource = (char*)s3eMalloc(sizeof(resource));
		pr.resource = std::string(resource);
		pr.sendType = CIwHTTP::DELETE;
		pr.userData = userData;
		pendingRequests().push_back(pr);

		return 0;
	}

	if (onGoingRequest && onGoingRequest->requestStatus == READY){
		delete onGoingRequest;
		onGoingRequest = NULL;
	}

	_onResult = onResult;
	_onError = onError;


	onGoingRequest = new RequestData(resource, http_object, CIwHTTP::DELETE, GotResult, NONE, NULL, userData);

	return 0;
}

int32 PhuneRestBase::_StartMatch(const char *gameId, s3eCallback onResult, s3eCallback onError, void *userData){

	

	char resource[200];
	std::memset(resource, 0, sizeof(resource));
	sprintf(resource, "/jamp/matches");

	if (!pendingRequests().empty() || onGoingRequest && onGoingRequest->requestStatus == ONGOING_REQUEST)
	{
		pendingRequest pr;
		pr.body = std::string(gameId);
		pr.onError = onError;
		pr.onResult = onResult;
		pr.resource = std::string(resource);
		pr.responseObjectType = PHUNE_MATCH_OBJECT;
		pr.sendType = CIwHTTP::POST;
		pr.userData = userData;
		pendingRequests().push_back(pr);

		return 0;
	}

	if (onGoingRequest && onGoingRequest->requestStatus == READY){
		delete onGoingRequest;
		onGoingRequest = NULL;
	}

	_onResult = onResult;
	_onError = onError;

	onGoingRequest = new RequestData(resource, http_object, CIwHTTP::POST, GotResult, PHUNE_MATCH_OBJECT, gameId, userData);
}
int32 PhuneRestBase::_EndMatch(int64 matchId, PhunePlayer player, s3eCallback onResult, s3eCallback onError, void *userData){
	char resource[200];
	std::memset(resource, 0, sizeof(resource));
	sprintf(resource, "/jamp/matches/%d/finish", matchId);

	IwTrace(PHUNE, ("End match:%s", player.Serialize().c_str()));
	if (!pendingRequests().empty() || onGoingRequest && onGoingRequest->requestStatus == ONGOING_REQUEST)
	{
		pendingRequest pr;

		pr.body = player.Serialize();
		pr.onError = onError;
		pr.onResult = onResult;
		pr.resource = std::string(resource);
		pr.responseObjectType = NONE;
		pr.sendType = CIwHTTP::PUT;
		pr.userData = userData;
		pendingRequests().push_back(pr);

		return 0;
	}

	if (onGoingRequest && onGoingRequest->requestStatus == READY){
		delete onGoingRequest;
		onGoingRequest = NULL;
	}

	_onResult = onResult;
	_onError = onError;

	onGoingRequest = new RequestData(resource, http_object, CIwHTTP::PUT, GotResult, NONE, player.Serialize().c_str(), userData);

}


int32 PhuneRestBase::GotResult(void *result, void *userData){
	RequestData *requestData = static_cast<RequestData*>(userData);

	bool isError = false;
	if (requestData && requestData->requestStatus == REQUEST_ERROR){
		isError = true;
	}

	if (requestData && requestData->responseObjectType == PHUNE_TIMESTAMP){
		server_time = atoll((char *)result);
		lib_time = clock();
		IwTrace(PHUNE, ("Clock server:%lld Clock lib:%lld", server_time, lib_time));
	}	

	if (requestData){
		//I could need this object
		CIwHTTP *httpObject = requestData->http_object;
		
		
		//continue to callback
		if (isError){
			PhuneRestBase::_onError(result, requestData->userData);
		}
		else{
			PhuneRestBase::_onResult(result, NULL);
		}
		requestData->requestStatus = READY;
		delete requestData;
		requestData = NULL;

		//empty List of pending requests
		if (pendingRequests().empty()){
			//has finished the pending requests
			IwTrace(PHUNE, ("There are no pending requests"));
		}
		//as at least one element
		else{
			//print the pending requests in order
			IwTrace(PHUNE, ("The content of pending requests are:"));
			for (std::deque<pendingRequest>::iterator it = pendingRequests().begin(); it != pendingRequests().end(); ++it){
				IwTrace(PHUNE, ("resource:%s", it->resource.c_str()));
			}

			//remove the first element of the list
			pendingRequest pr = pendingRequests().front();
			pendingRequests().pop_front();
			
			PhuneRestBase::_onResult = pr.onResult;
			PhuneRestBase::_onError = pr.onError;

			requestData = new RequestData(pr.resource.c_str(), httpObject, pr.sendType, GotResult, pr.responseObjectType, pr.body.c_str(), pr.userData);
		}
	}
	return 0;
}

int32 PhuneRestBase::_onHideWebView(s3eWebView *instance, void *systemData, void *userData){
	IwTrace(PHUNE, ("_onHideWebView"));
	PhuneRestBase *object = static_cast<PhuneRestBase*>(userData);
	
	
	std::string out = string(static_cast<const char*>(systemData));
	IwTrace(PHUNE, ("PhuneRestBase::_onHideWebView. Resource:%s", out.c_str()));

	if (out.find(LOGIN_REDIRECT) != string::npos){
		IwTrace(PHUNE, ("PhuneRestBase::onHideWebView Found login Redirect:%s", out.c_str()));
		
		int tmp = STOP_VIEW;
		_onResult(&tmp, NULL);

		std::string prefix = string();

		prefix.append(URL_SCHEMA);
		prefix.append("://");
		prefix.append(URL_HOST);
		prefix.append(URL_CONTEXT);


		size_t pos = out.find("#_=_");
		if (pos != string::npos){
			out.erase(pos);
		}
		
		IwTrace(PHUNE, ("PhuneRestBase::onHideWebView erasing %d chars:%s",prefix.length(), prefix.c_str()));
		out.erase(0, prefix.length());

		IwTrace(PHUNE, ("PhuneRestBase::onHideWebView after cleaning request:%s", out.c_str()));

		s3eWebViewUnRegister(S3E_WEBVIEW_FINISHED_LOADING, _onHideWebView, instance);

		object->onGoingRequest = new RequestData(out.c_str(), object->http_object, CIwHTTP::GET, object->GotResult, PHUNE_REDIRECT_LOGIN, NULL, NULL);

		
	}
	else{
		s3eWebViewUnRegister(S3E_WEBVIEW_FINISHED_LOADING, _onHideWebView, instance);
		s3eWebViewRegister(S3E_WEBVIEW_FINISHED_LOADING, _onHideWebView, userData, instance);
	}
	return 0;
}

//-----------------------------------------------------------------------------
// From request data
//-----------------------------------------------------------------------------
RequestData::RequestData(const char *resource, CIwHTTP *http_object, CIwHTTP::SendType sendType, s3eCallback onResult, ResponseObjectType responseObjectType, const char *body, void *userData)
{
	this->requestStatus = ONGOING_REQUEST;
	this->responseObjectType = responseObjectType;
	this->onResult = onResult;
	this->http_object = http_object;
	this->userData = userData;

	this->result = NULL;

	const char *URI = getUri(resource);
	
	switch (sendType)
	{
	case CIwHTTP::GET:
		IwTrace(PHUNE, ("Doing GET request:%s", URI));
		this->http_object->Get(URI, RequestData::GotHeaders, this);
		break;
	case CIwHTTP::POST:
		IwTrace(PHUNE, ("Doing POST request:%s", URI));
		this->http_object->Post(URI, body, strlen(body), RequestData::GotHeaders, this);
		break;
	case CIwHTTP::PUT:
		IwTrace(PHUNE, ("Doing PUT request:%s", URI));
		this->http_object->Put(URI, body, strlen(body), RequestData::GotHeaders, this);
		break;
	case CIwHTTP::DELETE:
		IwTrace(PHUNE, ("Doing DELETE request:%s", URI));
		this->http_object->Delete(URI, RequestData::GotHeaders, this);
		break;
	case CIwHTTP::HEAD:
	default:
		break;
	}

}

RequestData::~RequestData()
{
	if (http_object != NULL)
		if (!http_object->ContentFinished())
			http_object->Cancel();

	try{
		if (result && result != NULL)
		{
			char **resultBuffer = &(result);
			s3eFree(*resultBuffer);
		}
	}
	catch (int e){

	}
	
}

//-----------------------------------------------------------------------------
// Called when the response headers have been received
//-----------------------------------------------------------------------------
int32 RequestData::GotHeaders(void*, void *userData)
{

	RequestData *requestData = static_cast<RequestData*>(userData);

	IwTrace(PHUNE, ("on got headers INIT"));
	
	if (requestData->http_object->GetStatus() == S3E_RESULT_ERROR || requestData->http_object->GetResponseCode() >= 400)
	{
		IwTrace(PHUNE, ("on got headers ERROR"));
		requestData->requestStatus = REQUEST_ERROR;
		requestData->onResult(new RequestError(REQUEST_ERROR, requestData->http_object->GetResponseCode()), requestData);
		return 0;
	}
	if (requestData->responseObjectType == NONE || requestData->http_object->GetResponseCode() == 204){
		IwTrace(PHUNE, ("on got headers NONE or No content"));
		requestData->onResult(NULL, requestData);
	}
	else
	{
		std::string strCookie;
		bool header_response = requestData->http_object->GetHeader("X-Auth-Token", strCookie);
		if (header_response){
			s3eFile*  cookie_file = s3eFileOpen("phune_cookie.cookie", "w");
			if (cookie_file){
				IwTrace(PHUNE, ("Saving Cookie file"));
				s3eFilePrintf(cookie_file, strCookie.c_str());

				s3eFileClose(cookie_file);
			}
			else{
				IwTrace(PHUNE, ("Could not create Cookie file"));
			}
			IwTrace(PHUNE, ("Setting Cookie in session %s", strCookie.c_str()));

			//deprecated to remove after server supports the authorization header
			std::string cookie_prefix = "JSESSIONID=";
			cookie_prefix.append(strCookie.c_str());
			requestData->http_object->SetRequestHeader("Cookie", cookie_prefix.c_str());

			std::string authorization_prefix = "Bearer ";
			authorization_prefix.append(strCookie.c_str());
			requestData->http_object->SetRequestHeader("Authorization", authorization_prefix.c_str());
		}


		requestData->len = requestData->http_object->ContentExpected();
		if (!requestData->len)
		{
			requestData->len = 1024;
		}

		char **resultBuffer = &(requestData->result);

		s3eFree(*resultBuffer);

		*resultBuffer = (char*)s3eMalloc(requestData->len + 1);
		std::memset(*resultBuffer, 0, requestData->len + 1);

		(*resultBuffer)[requestData->len] = 0;


		IwTrace(PHUNE, ("on got headers"));

		requestData->http_object->ReadDataAsync(*resultBuffer, requestData->len, REQUEST_TIMEOUT, GotData, requestData);
	}

	return 0;
}

int32 RequestData::GotData(void*, void *userData)
{
	
	RequestData *requestData = static_cast<RequestData*>(userData);

	if (!requestData->http_object->ContentFinished()){
		IwTrace(PHUNE, ("Still not finished"));
		return 0;
	}
	// This is the callback indicating that a ReadContent call has
	// completed.  Either we've finished, or a bigger buffer is
	// needed.  If the correct ammount of data was supplied initially,
	// then this will only be called once. However, it may well be
	// called several times when using chunked encoding.

	// Firstly see if there's an error condition.
	if (requestData->http_object->GetStatus() == S3E_RESULT_ERROR || requestData->http_object->GetResponseCode()>=400)
	{
		requestData->requestStatus = REQUEST_ERROR;
		requestData->onResult(new RequestError(REQUEST_ERROR, requestData->http_object->GetResponseCode()), requestData->userData);
	}
	else if (requestData->http_object->ContentReceived() != requestData->http_object->ContentExpected())
	{
		// We have some data but not all of it. We need more space.
		uint32 oldLen = requestData->len;
		// If iwhttp has a guess how big the next bit of data is (this
		// basically means chunked encoding is being used), allocate
		// that much space. Otherwise guess.
		if (requestData->len < requestData->http_object->ContentExpected())
			requestData->len = requestData->http_object->ContentExpected();
		else
			requestData->len += 1024;

		IwTrace(PHUNE, ("on got data"));

		// Allocate some more space and fetch the data.
		requestData->result = (char*)s3eRealloc(requestData->result, requestData->len);
		requestData->http_object->ReadContent(&(requestData->result[oldLen]), requestData->len - oldLen, GotData);
	}
	else
	{
		// We've got all the data. Display it.
		//status = kOK;



		IwTrace(PHUNE, ("Received data:\n%s", requestData->result));
		PhuneUser *pu;
		PhunePreferenceBase64 *ppb;
		PhunePreference *pp;
		PhuneMatch *pm;

		int tmp = FINISHED;


		switch (requestData->responseObjectType)
		{
		case PHUNE_USER_OBJECT:
			pu = new PhuneUser();
			pu->Deserialize(std::string(requestData->result));
			requestData->onResult(pu, requestData);
			break;
		case PHUNE_PREFERENCE_OBJECT:
			ppb = new PhunePreferenceBase64();
			ppb->Deserialize(std::string(requestData->result));
			requestData->onResult(ppb, requestData);
			break;
		case PHUNE_PREFERENCE_JSON:
		case PHUNE_PREFERENCE_JSON_LIST:
			pp = new PhunePreference();
			pp->Deserialize(std::string(requestData->result));
			requestData->onResult(pp, requestData);
			break;
		case PHUNE_TIMESTAMP:
			requestData->onResult(requestData->result, requestData);
			break;
		case PHUNE_MATCH_OBJECT:
			pm = new PhuneMatch();
			pm->Deserialize(std::string(requestData->result));
			requestData->onResult(pm, requestData);
			break;
		case PHUNE_REDIRECT_LOGIN:
			requestData->onResult(&tmp, requestData);
			break;
		default:
			requestData->requestStatus = REQUEST_ERROR;
			requestData->onResult(new RequestError(INVALID_RESPONSE_OBJECT, 0), requestData);
			break;
		}



	}
	return 0;
}

const char* RequestData::getUri(const char *resource)
{
	std::string  *uri = new std::string();

	uri->append(URL_SCHEMA);
	uri->append("://");
	uri->append(URL_HOST);
	//uri->append(":");
	//uri->append(URL_PORT);
	uri->append(URL_CONTEXT);
	uri->append(resource);

	char out[200];
	std::memset(out, 0, sizeof(out));
	strncpy(out, uri->c_str(), uri->length());


	delete uri;

	return out;
}


RequestError::RequestError(RequestStatus type, uint32 httpCode){
	this->type = type;
	this->httpCode = httpCode;
}

RequestError::~RequestError()
{

}

