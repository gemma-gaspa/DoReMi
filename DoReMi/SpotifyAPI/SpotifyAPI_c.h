#ifndef GASPASPOTIFYAPI_C_H
#define GASPASPOTIFYAPI_C_H

// ******************************************************************
// ******************************************************************

// Proj
#include "SpotifyUserSecrets_c.h"

// C/C++ Libs
#include <map>
#include <vector>

// Qt Libs
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QTimer>


// ******************************************************************
// ******************************************************************
class SpotifyAPI_c : public QObject
{
	Q_OBJECT
public:
	SpotifyAPI_c(/*QObject* aopQObject = nullptr*/);
	virtual ~SpotifyAPI_c();

	uint32_t mvSetConnection(const SpotifyUserSecrets_c& aorSpotifyUserSecrets, uint32_t auTimeout_ms);

	bool mvEnabled() const ;


	// Do a or of the intrested flags
	enum class Flags_e {
			eIS_PLAYABLE_30s = 0x1
	};

	struct SearchTrackItems_s {
		// Requested types:
		struct Artist_s {
	//		external_urls
	//		href" : "https://api.spotify.com/v1/artists/2ye2Wgw4gimLv2eAKyk1NB",
	//		id"   : "2ye2Wgw4gimLv2eAKyk1NB",
			QString sName ;
			QString sType ;
	//		uri"  : "spotify:artist:2ye2Wgw4gimLv2eAKyk1NB"
		} ;
		struct Images_s  {
			int      iHeight =0 ;
			int      iWidth  =0  ;
			QString  sUrl ;
		} ;

		// Data
		struct Album_s {
			QString sAlbum_type ;
	//		std::vector<Artists_s> ovArtists;
	//		QString external_urls;
	//		href
	//		id
			std::vector<Images_s> ovImages;
			QString sName;
			QString sReleaseDate;
	//		release_date_precision
	//		total_tracks
	//		QString sType;
	//		QString sUri;
		} oAlbum ;
		std::vector<Artist_s> ovArtists;
	//	std::vector<QString> svAvailable_markets;
	//	int  iDisc_number =0;
		int  iDuration_ms =0;
	//	bool bExplicitLyrics =false;
	//	external_ids
	//	external_url
	//	href
		QString     sId;
	//	bool      bIs_playable
	//	linked_from
	//	restrictions
		QString     sName;
		int         iPopularity =0;
		QByteArray  sPreview_url;
	//	int         iTrack_number =0;
		QString     sType;
	//	QString     sUri ;
	};



//	using ResultTrackSearch_x = std::vector<SpotifyAPI_c::SearchTrackItems_s>;

	void mvSetTimeQuantaWait(uint32_t auTimeQuantaSleep) ;

	uint32_t movSearchTrack(const QString& asrExpression,
			uint32_t auNumTracks,
			uint32_t auSearchFlags,
			std::vector<SearchTrackItems_s>& avrReturnSearch);

	enum class ConnectionStatus_e {
		eDISABLE,
		eOK,
		eERROR_BEARER,
		eTIMEOUT,
		eTLS_INIT_FAIL,
		ePARSER_ERROR,
	};

	ConnectionStatus_e meGetConnectionStatus() const ;

private slots:
	void     mvRenewTokenSlot() ;

private:
	uint32_t muTimeout_ms = 3500 ;

	uint32_t mvRenewTokenCommon() ;

	const uint32_t muMAX_SEARCH_TRACKS = 50;

	struct SearchStepInfo_s {
		QString sPrevious ;
		QString sNext ;

		int iLimit=0;
		int iOffset=0;
		int iTotal=0;
	};
	uint32_t movSearchTrackAuxiliar(
			const QString& asrExpression,
			uint32_t auNumTracks,
			uint32_t auOffestTrack,
			std::vector<SearchTrackItems_s>& avrReturn,
			SearchStepInfo_s& aorSearchStep);


	ConnectionStatus_e meConnectionStatus = ConnectionStatus_e::eDISABLE ;

	const QString msEndpointAuth ;
	const QString msEndpointSearch ;

	enum class Endpoints_e {

	};

	const static QString msApiBase ; // link de acesso

	uint32_t muTimeQuantaWait = 20 ;

	struct ReturnMsg {
		QString  cShortMsg;
		QString  cExplain;
	} ;

	const static std::map<uint16_t, ReturnMsg> moReturnCodes;


	enum class ConnectionState_e {
		NONE,
		OK,
		DENIED
	};

	// Connection data:
	SpotifyUserSecrets_c moSpotifyUserSecrets;
	QByteArray msToken ;
	QDateTime  moLastUpdate;

	bool mbEnableSpotifyAPI = false ;

	int miLifeTime_ms = 3600*1000; // valor padrao da documentacao
	QTimer moTimerRenewToken ;
};

#endif // GASPASPOTIFYAPI_C_H
