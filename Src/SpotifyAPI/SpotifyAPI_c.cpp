

#include "SpotifyAPI_c.h"

// C/C++ Libs
#include <map>
#include <vector>

// Qt Libs
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QTest>
#include <QTimer>
#include <QUrlQuery>


// ******************************************************************
// ****************************************************************************
const std::map<uint16_t, SpotifyAPI_c::ReturnMsg> SpotifyAPI_c::moReturnCodes = {
	{200, {"OK"                    , "The request has succeeded. The client can read the result of the request in the body and the headers of the response."}},
	{201, {"Created"               , "The request has been fulfilled and resulted in a new resource being created."}},
	{202, {"Accepted"              , "The request has been accepted for processing, but the processing has not been completed."}},
	{204, {"No Content"            , "The request has succeeded but returns no message body."}},
	{304, {"Not Modified"          , "See Conditional requests."}},
	{400, {"Bad Request"           , "The request could not be understood by the server due to malformed syntax. The message body will contain more information; see Response Schema."}},
	{401, {"Unauthorized"          , "The request requires user authentication or, if the request included authorization credentials, authorization has been refused for those credentials."}},
	{403, {"Forbidden"             , "The server understood the request, but is refusing to fulfill it."}},
	{404, {"Not Found"             , "The requested resource could not be found. This error can be due to a temporary or permanent condition."}},
	{429, {"Too Many Requests"     , "Rate limiting has been applied."}},
	{500, {"Internal Server Error" , "You should never receive this error because our clever coders catch them all … but if you are unlucky enough to get one, please report it to us through a comment at the bottom of this page."}},
	{502, {"Bad Gateway"           , "The server was acting as a gateway or proxy and received an invalid response from the upstream server."}},
	{503, {"Service Unavailable"   , "The server is currently unable to handle the request due to a temporary condition which will be alleviated after some delay. You can choose to resend the request again."}}
};

//const QString SpotifyAPI_c::msApiBase = "https://api.spotify.com";


const QString SpotifyAPI_c::msEndpointAuth("https://accounts.spotify.com/api/token");
const QString SpotifyAPI_c::msEndpointSearch("https://api.spotify.com/v1/search");
const QString SpotifyAPI_c::msEndpointTrack("https://api.spotify.com/v1/tracks");

// ****************************************************************************
SpotifyAPI_c::SpotifyAPI_c()
{
	//      source              signal            dest obj  function to execute (slot)
	connect(&moTimerRenewToken, &QTimer::timeout, this    , &SpotifyAPI_c::mvRenewTokenSlot); // signal para renovar token, depois de menos que 1h
}


// ****************************************************************************
SpotifyAPI_c::~SpotifyAPI_c()
{
	// Fechar conexoes?

	// Liberar memorias?
}


// ****************************************************************************
// Inicia a conexao com o Spotify, ao passar
// Retorno: tempo de resposta
uint32_t
SpotifyAPI_c::mvSetConnection(const SpotifyUserSecrets_c& aorSpotifyUserSecrets, uint32_t auTimeout_ms)
{
	muTimeout_ms = auTimeout_ms;
	moSpotifyUserSecrets        = aorSpotifyUserSecrets ;
	moSpotifyUserSecrets.ID     = moSpotifyUserSecrets.ID ;
	moSpotifyUserSecrets.Secret = moSpotifyUserSecrets.Secret ;

	return SpotifyAPI_c::mvRenewTokenCommon() ;
}


// ****************************************************************************
SpotifyAPI_c::ConnectionStatus_e
SpotifyAPI_c::meGetConnectionStatus()
const
{
	// Fazer algum teste de conexao
	return meConnectionStatus ;
}


// ****************************************************************************
QString
SpotifyAPI_c::msGetLastError()
const
{
	return msLastError ;
}


// ****************************************************************************
// retorno: tempo de procura
uint32_t
SpotifyAPI_c::movSearchTrackAuxiliar(
		const QString&       asrExpression,
		uint32_t             auNumTracks,
		uint32_t             auOffestTrack,
		std::vector<SearchTrackItems_s>& avrReturnTracks,
		SearchStepInfo_s&    aorSearchStep)
{
	QNetworkAccessManager *opWebManager = new QNetworkAccessManager(this); // tirar de ponteiro

	QString sError ;
	bool bReturned = false ;
	bool bFinished = false ;

	QByteArray sContent ;

	QTimer oTimeOut;
	//      source         signal                           dest          function to execute
	connect(opWebManager, &QNetworkAccessManager::finished, opWebManager, &QNetworkAccessManager::deleteLater);
	connect(opWebManager, &QNetworkAccessManager::finished, this        ,
			// Lambda Construct! :-)
			[&sError, &bReturned, &bFinished, &oTimeOut, &sContent]
			(QNetworkReply* aopReply)
			{
				bFinished = aopReply->isFinished() ;
				sError    = aopReply->errorString();
				bReturned = true;
				sContent  = aopReply->readAll() ;
				oTimeOut.stop(); // ultimo evento nesta funcao
			});

	// Adiciona queries
	QUrl oURL(msEndpointSearch);
	QUrlQuery oQuery;

	oQuery.addQueryItem("q"     , asrExpression);
	oQuery.addQueryItem("limit" , QString::number( std::min(muMAX_SEARCH_TRACKS, auNumTracks) ));
	oQuery.addQueryItem("offset", QString::number(auOffestTrack));
	oQuery.addQueryItem("type"  , "track");
	oQuery.addQueryItem("market", "BR");
	oURL.setQuery(oQuery);

	// Adiciona Headers e faz requisicao get
	// https://developer.spotify.com/documentation/web-api/reference/search/search/
	QNetworkRequest oWebRequest(oURL) ;
	// curl -X "GET"
	// https://api.spotify.com/v1/search?q=lazy&type=track
	// -H "Accept: application/json"
	// -H "Content-Type: application/json"
	// -H "Authorization: Bearer BQAV4DAkHQWn6lXjdHvQ3wvrLbfIro5nwhZhxQZudZQDJIc3IifxzWbSwAbbvfNOIP0iXAttDadXkaQsvPbPbzd5wlqrFEDbVpvxvjplowmSxh5ZvhxClzsVe5xl-pch8cPMTpauj02UxjLaVxHrGvI"
	oWebRequest.setRawHeader("Accept:", "application/json");
	oWebRequest.setRawHeader("Content-Type:", "application/json");
	oWebRequest.setRawHeader("Authorization:", "Bearer " + msToken);
	opWebManager->get( oWebRequest );

	oTimeOut.start(  int(muTimeout_ms)  ); // Inicia contagem de timeout
	uint32_t uNumIter= 0 ;

	for( ; uNumIter<(muTimeout_ms/muTimeQuantaWait_ms) && !bReturned; uNumIter++) {
		QTest::qWait(int(muTimeQuantaWait_ms)); // Dorme sem paralisar a lambda por 100ms
	}

	if(bReturned) {
		QJsonParseError oErrorParser;
		QJsonDocument   oJsonDoc    = QJsonDocument::fromJson(sContent, &oErrorParser);
		QJsonObject     oJsonTracks = oJsonDoc["tracks"].toObject();
		if (!oJsonDoc.isNull()) {
			aorSearchStep.iLimit   = oJsonTracks["limit"   ].toInt();
			aorSearchStep.iOffset   = oJsonTracks["offset"  ].toInt();
			aorSearchStep.iTotal    = oJsonTracks["total"   ].toInt();
			aorSearchStep.sNext     = oJsonTracks["next"    ].toString();
			aorSearchStep.sPrevious = oJsonTracks["previous"].toString();

			QJsonArray oArrayTracks = oJsonTracks["items"].toArray();

			for (auto itT : oArrayTracks) {
				QJsonObject oObjT = itT.toObject();
				SearchTrackItems_s oItemTrack;

				oItemTrack.iDuration_ms = oObjT["duration_ms"].toInt() ;
				oItemTrack.sId          = oObjT["id"         ].toString();
				oItemTrack.sName        = oObjT["name"       ].toString();
				oItemTrack.iPopularity  = oObjT["popularity" ].toInt();
				oItemTrack.sPreview_url = oObjT["preview_url"].toString().toUtf8() ;
				oItemTrack.sType        = oObjT["type"       ].toString();

				QJsonObject oObjAlbum = oObjT["album"].toObject();
				oItemTrack.oAlbum.sAlbum_type  = oObjAlbum["album_type"  ].toString();
				oItemTrack.oAlbum.sName        = oObjAlbum["name"        ].toString();
				oItemTrack.oAlbum.sReleaseDate = oObjAlbum["release_date"].toString();

				QJsonArray oArrayImages = oObjAlbum["images"].toArray();
				for (auto itIm : oArrayImages) {
					QJsonObject oObjImg = itIm.toObject();
					SearchTrackItems_s::Images_s oItemImg;
					oItemImg.iHeight = oObjImg["height"].toInt();
					oItemImg.iWidth  = oObjImg["width" ].toInt();
					oItemImg.sUrl    = oObjImg["url"   ].toString();

					oItemTrack.oAlbum.ovImages.push_back(oItemImg);
				}

				QJsonArray oArrayArtists = oObjT["artists"].toArray();
				for (auto itA : oArrayArtists) {
					QJsonObject oObjA = itA.toObject();
					SearchTrackItems_s::Artist_s oItemArt;
					oItemArt.sName = oObjA["name"].toString();
					oItemArt.sType = oObjA["type"].toString();

					oItemTrack.ovArtists.push_back(oItemArt);
				}

				avrReturnTracks.push_back(oItemTrack);
			}
		} else {
			msLastError = "Json Parser failure :" +oErrorParser.errorString() ;
		}
	} else {
		msLastError = "Timeout" ;
	}

	if(sError!="Unknown error") {
		msLastError += "\n   " + sError ;
	}

	uint32_t uTimeRet_ms = uNumIter*muTimeQuantaWait_ms ;
	return uTimeRet_ms ;
}


// ****************************************************************************
// retorno: tempo de procura
uint32_t
SpotifyAPI_c::movSearchTrack(
		const QString& asrExpression,
		uint32_t auNumTracks,
		uint32_t auSearchFlags,
		std::vector<SearchTrackItems_s>& avrReturnSearch)
{
	uint32_t uSumTime= 0 ;
	avrReturnSearch.clear();
	if(ConnectionStatus_e::eOK == meConnectionStatus) {

		SearchStepInfo_s oSearchStep ;

		uint32_t uOffestTrack=0;

		bool bStop = false;
		do {
			uint32_t uTime = 0;
			std::vector<SearchTrackItems_s> ovResultTemp;
			uTime = SpotifyAPI_c::movSearchTrackAuxiliar(
						asrExpression,
						muMAX_SEARCH_TRACKS,
						uOffestTrack,
						ovResultTemp,
						oSearchStep) ;

			// Includes only filtered by the flags.
			for(uint16_t u=0 ; u<ovResultTemp.size() && !bStop ; u++){
				// Nand entre
				bool bIns = true ;
				bool bHavePreview = (ovResultTemp[u].sPreview_url != "");

				if(!bHavePreview) {
					ovResultTemp[u].sPreview_url = msWorkaround___preview_url___(ovResultTemp[u].sId);
				}


				if(auSearchFlags!=0) {
					bIns = (auSearchFlags & uint32_t(Flags_e::eIS_PLAYABLE_30s)) && bHavePreview;
				}
				if (bIns){
					avrReturnSearch.push_back(ovResultTemp[u]) ;
					bStop |= avrReturnSearch.size() == auNumTracks ; // Encheu
				}
			}
			uSumTime     += uTime;
			uOffestTrack += uint32_t(oSearchStep.iLimit);
			bStop |= ovResultTemp.size() < muMAX_SEARCH_TRACKS ;  // Nao tem mais

			// inserir até encher
		} while (!bStop) ;
	}

	return uSumTime ;
}


// ****************************************************************************
void
SpotifyAPI_c::mvSetTimeQuantaWait(uint32_t auTimeQuantaWait)
{
	muTimeQuantaWait_ms = auTimeQuantaWait;
}


// ****************************************************************************
void
SpotifyAPI_c::mvRenewTokenSlot()
{
	mvRenewTokenCommon() ;
}


// ****************************************************************************
// Chamada por mvRenewTokenSlot() ou por mvSetConnection()
// Client Credentials Flow
uint32_t
SpotifyAPI_c::mvRenewTokenCommon() {
	// Concatena strings, conforme doc da API
	QByteArray sKey =  (moSpotifyUserSecrets.ID + ":" + moSpotifyUserSecrets.Secret).toBase64(); // Codifica para Base64
	QNetworkAccessManager *opWebManager = new QNetworkAccessManager(this); // tirar de ponteiro

	QString sError ;
	bool bReturned = false ;
	bool bFinished = false ;

	QByteArray sContent ;

	QTimer oTimeOut;
	//      source         signal                           dest          function to execute
	connect(opWebManager, &QNetworkAccessManager::finished, opWebManager, &QNetworkAccessManager::deleteLater);
	connect(opWebManager, &QNetworkAccessManager::finished, this        ,
			// Lambda Construct! :-)
			[&sError, &bReturned, &bFinished, &oTimeOut, &sContent]
			(QNetworkReply* aopReply)
			{
				bFinished = aopReply->isFinished() ;
				sError    = aopReply->errorString();
				bReturned = true;
				sContent = aopReply->readAll() ;
				oTimeOut.stop(); // ultimo evento nesta funcao
			});

	// Adiciona queries
	QUrl oURL(msEndpointAuth);
	QUrlQuery oQuery;
	oQuery.addQueryItem("grant_type", "client_credentials");
	oURL.setQuery(oQuery);

	// Monta Headers e faz requisicao post
	QNetworkRequest oWebRequest(oURL) ;
	oWebRequest.setRawHeader("Authorization", "Basic " + sKey);
	QByteArray sReturnPost ; // verificar importancia desta coisa
	opWebManager->post( oWebRequest, sReturnPost );

	oTimeOut.start(  int(muTimeout_ms)  ); // Inicia contagem de timeout

	uint32_t uNumIter= 0 ; // Iteracoes de 100ms
	for( ; uNumIter<(muTimeout_ms/muTimeQuantaWait_ms) && !bReturned; uNumIter++) {
		QTest::qWait(int(muTimeQuantaWait_ms)); // Dorme sem paralisar a lambda por 100ms
	}

	if(bReturned) {
		if(sError == "TLS initialization failed") {
			meConnectionStatus = ConnectionStatus_e::eTLS_INIT_FAIL;
			msLastError = "TLS initialization failed" ;

		} else {
			QJsonParseError oErrorParser;
			QJsonDocument   oJsonDoc = QJsonDocument::fromJson(sContent, &oErrorParser);
			if (!oJsonDoc.isNull()) {
				msToken            = oJsonDoc["access_token"].toString().toUtf8();
				QString sTokenType = oJsonDoc["token_type"  ].toString(); // Espera: "Bearer"
				miLifeTime_ms      = oJsonDoc["expires_in"  ].toInt()*(1000*3)/(4*1); // fire on 3/4 of the time
				QString x4         = oJsonDoc["scope     "  ].toString();

				//moTimerRenewToken.setInterval(miLifeTime_ms);
				meConnectionStatus = ("Bearer" == sTokenType) ? ConnectionStatus_e::eOK :ConnectionStatus_e::eERROR_BEARER ;
				moTimerRenewToken.start(miLifeTime_ms) ; // E vamos nos de novo...

			} else {
				meConnectionStatus = ConnectionStatus_e::ePARSER_ERROR;
				msLastError = "Json Parser failure :" +oErrorParser.errorString() ;
			}
		}

	} else  {
		meConnectionStatus = ConnectionStatus_e::eTIMEOUT;
	}

	if(sError!="Unknown error") {
		msLastError += "\n   " + sError ;
	}

	uint32_t uTimeRet_ms = uNumIter*muTimeQuantaWait_ms ;
	return uTimeRet_ms ;
}


// ****************************************************************************
QByteArray
SpotifyAPI_c::msWorkaround___preview_url___(QString asTrackID)
{
	QNetworkAccessManager *opWebManager = new QNetworkAccessManager(this); // tirar de ponteiro

	QString sError ;
	bool bReturned = false ;
	bool bFinished = false ;

	QByteArray sContent ;

	QTimer oTimeOut;
	//      source         signal                           dest          function to execute
	connect(opWebManager, &QNetworkAccessManager::finished, opWebManager, &QNetworkAccessManager::deleteLater);
	connect(opWebManager, &QNetworkAccessManager::finished, this        ,
			// Lambda Construct! :-)
			[&sError, &bReturned, &bFinished, &oTimeOut, &sContent]
			(QNetworkReply* aopReply)
			{
				bFinished = aopReply->isFinished() ;
				sError    = aopReply->errorString();
				bReturned = true;
				sContent  = aopReply->readAll() ;
				oTimeOut.stop(); // ultimo evento nesta funcao
			});

	// Adiciona queries
	QUrl oURL(msEndpointTrack+'/'+asTrackID);
//	QUrlQuery oQuery;

//	oQuery.addQueryItem("id" , asTrackID);
//	oURL.setQuery(oQuery);

	// Adiciona Headers e faz requisicao get
	// https://developer.spotify.com/documentation/web-api/reference/search/search/
	QNetworkRequest oWebRequest(oURL) ;
	// curl -X "GET"
	// https://api.spotify.com/v1/search?q=lazy&type=track
	// -H "Accept: application/json"
	// -H "Content-Type: application/json"
	// -H "Authorization: Bearer BQAV4DAkHQWn6lXjdHvQ3wvrLbfIro5nwhZhxQZudZQDJIc3IifxzWbSwAbbvfNOIP0iXAttDadXkaQsvPbPbzd5wlqrFEDbVpvxvjplowmSxh5ZvhxClzsVe5xl-pch8cPMTpauj02UxjLaVxHrGvI"
	oWebRequest.setRawHeader("Accept:", "application/json");
	oWebRequest.setRawHeader("Content-Type:", "application/json");
	oWebRequest.setRawHeader("Authorization:", "Bearer " + msToken);
	opWebManager->get( oWebRequest );

	oTimeOut.start(  int(muTimeout_ms)  ); // Inicia contagem de timeout
	uint32_t uNumIter= 0 ;

	for( ; uNumIter<(muTimeout_ms/muTimeQuantaWait_ms) && !bReturned; uNumIter++) {
		QTest::qWait(int(muTimeQuantaWait_ms)); // Dorme sem paralisar a lambda por 100ms
	}

	QByteArray sRet ;

	if(bReturned) {
		QJsonParseError oErrorParser;
		QJsonDocument   oJsonDoc    = QJsonDocument::fromJson(sContent, &oErrorParser);
		if (!oJsonDoc.isNull()) {
			sRet = oJsonDoc["preview_url"].toString().toUtf8();

		} else {
			msLastError = "Json Parser failure :" + oErrorParser.errorString() ;
		}
	}

	return sRet ;
}


