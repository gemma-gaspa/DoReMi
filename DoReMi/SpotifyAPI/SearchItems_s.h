#ifndef SEARCHITEMS_S_H
#define SEARCHITEMS_S_H

#include <vector>


#include <QString>
struct TrackItems_s {
	// Requested types:
	struct Artists_s {
//		external_urls
//		href" : "https://api.spotify.com/v1/artists/2ye2Wgw4gimLv2eAKyk1NB",
//		id"   : "2ye2Wgw4gimLv2eAKyk1NB",
		QString sName ;
		QString sType ;
//		uri"  : "spotify:artist:2ye2Wgw4gimLv2eAKyk1NB"
	} ;
	struct Images_s  {
		int      iHeight ;
		int      iWidth  ;
		QString  sUrl    ;
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
		QString sRelease_date;
//		release_date_precision
//		total_tracks
//		QString sType;
//		QString sUri;
	} oAalbum ;
	std::vector<Artists_s> ovArtists;
//	std::vector<QString> svAvailable_markets;
	int  iDisc_number;
	int  iDuration_ms;
	bool bExplicit;
	//external_ids
	//external_url
	//href
	QString     sId;
//	bool      bIs_playable
//	linked_from
//	restrictions
	QString     sName;
	int         iPopularity;
	QByteArray  sPreview_url;
	int         iTrack_number;
	QString     sType;
	QString     sUri ;
};


struct SearchStepInfo_s {
	QString sPrevious ;
	QString sNext ;

	int iLlimit;
	int iOffset;
	int iTotal;
};

struct SearchTrackItems_s {




	// ------------------
	std::vector<TrackItems_s> ovItems ;



	// ------------------
	void TestFromFile() ;

	void Add(QString& asrStr);
};

#endif // SEARCHITEMS_S_H
