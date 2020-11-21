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

	struct SearchResult_s {

	};

	using ResultSearch_x = std::vector<SpotifyAPI_c::SearchResult_s>;

	void mvSetTimeQuantaWait(uint32_t auTimeQuantaSleep) ;

	uint32_t movSearchMusic(const QString& asrExpression, ResultSearch_x& avrReturn);

	enum class ConnectionStatus_e {
		eDISABLE,
		eOK,
		eERROR
	};

	ConnectionStatus_e meGetConnectionStatus() const ;

private:
	void mvRenewTokenSlot() ;
	uint32_t muTimeout_ms = 3500 ;
	uint32_t mvRenewTokenCommon() ;


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

	int miLifeTime = 3600;
	QTimer moRenewToken ;
};

#endif // GASPASPOTIFYAPI_C_H
