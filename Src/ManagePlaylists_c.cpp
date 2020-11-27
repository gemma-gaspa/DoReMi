
// Prj
#include "ManagePlaylists_c.h"

// C/C++
#include <vector>

// Qt
#include <QComboBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


const char ManagePlaylists_c::msCryptoTest[] ="123";


// **************************************************************************
ManagePlaylists_c::ManagePlaylists_c()
{
//	mvFromFile();
//	mvToFile();
}


// **************************************************************************
void
ManagePlaylists_c::mvFromFile()
{
	QFile sDataFile(msFileName);
	bool b = sDataFile.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Unbuffered);
	QByteArray sRawDataFile = sDataFile.readAll();
	sDataFile.close();

	QJsonParseError oErrorParser;
	QJsonDocument   oJsonDoc = QJsonDocument::fromJson(sRawDataFile, &oErrorParser);

	movUsersData.clear();
	if (!oJsonDoc.isNull()) {
		QJsonArray oArrayUsers = oJsonDoc.array() ;
		foreach(const QJsonValue& oJsonElem, oArrayUsers) {
			UsersData_s oUser;
			oUser.sName = oJsonElem["sName"].toString();

			QJsonArray oArrayPlaylists = oJsonElem["vPlaylists"].toArray();
			foreach(const QJsonValue& oJsonElemPlaylist, oArrayPlaylists) {
				UsersData_s::Playlists_s oPlaylist ;
				oPlaylist.sName = oJsonElemPlaylist["sName"].toString();

				QJsonArray oArrayTracks = oJsonElemPlaylist["vTracks"].toArray();
				foreach(const QJsonValue& oJsonElemTrack, oArrayTracks) {
					UsersData_s::Playlists_s::Track_s oTrack ;
					oTrack.sTrackName   = oJsonElemTrack["sTrackName"  ].toString();
					oTrack.sAlbumName   = oJsonElemTrack["sAlbumName"  ].toString();
					oTrack.sArtist      = oJsonElemTrack["sArtist"     ].toString();
					oTrack.sImage       = oJsonElemTrack["sImage"      ].toString();
					oTrack.sPreviewURL  = oJsonElemTrack["sPreviewURL" ].toString();
					oTrack.sReleaseDate = oJsonElemTrack["sReleaseDate"].toString();

					oPlaylist.ovTracks.push_back(oTrack);
				}
				oUser.ovPlaylists.push_back(oPlaylist);
			}
			movUsersData.push_back(oUser) ;
		}
	}

	// pra tela
	mvDataToPlaylistTable() ;
	mvDataToComboBox();
}


// **************************************************************************
void
ManagePlaylists_c::mvToFile()
{
	QJsonArray oJsonArrayUsers ;
	for(uint16_t uUsr=0 ; uUsr<movUsersData.size() ; uUsr++) { // sim, eu sei que da pra usar iterator.
		QJsonObject oJsonUserData ;
		oJsonUserData["sName"] = movUsersData[uUsr].sName;

		QJsonArray oJsonArrayPlaylists ;
		for(uint16_t uPl=0 ; uPl<movUsersData[uUsr].ovPlaylists.size(); uPl++) {
			QJsonObject oJsonPlaylist ;
			oJsonPlaylist["sName"] = movUsersData[uUsr].ovPlaylists[uPl].sName;

			QJsonArray oJsonArrayTracks ;
			for(uint16_t uTr=0 ; uTr<movUsersData[uUsr].ovPlaylists[uPl].ovTracks.size(); uTr++) {
				QJsonObject oJsonTrackData ;
				oJsonTrackData["sTrackName"  ] = movUsersData[uUsr].ovPlaylists[uPl].ovTracks[uTr].sTrackName;
				oJsonTrackData["sAlbumName"  ] = movUsersData[uUsr].ovPlaylists[uPl].ovTracks[uTr].sAlbumName;
				oJsonTrackData["sArtist"     ] = movUsersData[uUsr].ovPlaylists[uPl].ovTracks[uTr].sArtist;
				oJsonTrackData["sImage"      ] = movUsersData[uUsr].ovPlaylists[uPl].ovTracks[uTr].sImage;
				oJsonTrackData["sPreviewURL" ] = movUsersData[uUsr].ovPlaylists[uPl].ovTracks[uTr].sPreviewURL;
				oJsonTrackData["sReleaseDate"] = movUsersData[uUsr].ovPlaylists[uPl].ovTracks[uTr].sReleaseDate;

				oJsonArrayTracks.append(oJsonTrackData);
				//qMakePair(QString("sClient_ID"    ), QJsonValue(QString(oElem.sClient_ID    ))),
			}


			oJsonPlaylist["vTracks"] = oJsonArrayTracks;
			oJsonArrayPlaylists.append(oJsonPlaylist);

		}

		oJsonUserData["vPlaylists"] = oJsonArrayPlaylists;
		oJsonArrayUsers.append(oJsonUserData);
	}

	QByteArray sRawDataFile = QJsonDocument(oJsonArrayUsers).toJson() ;

	QFile sDataFile( msFileName ) ;
	sDataFile.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Unbuffered ) ;
	sDataFile.write( sRawDataFile ) ;
	sDataFile.close() ;
}


// **************************************************************************
void
ManagePlaylists_c::mvAddUser(const QString& aorName)
{
	UsersData_s oNewUser ;
	oNewUser.sName = aorName;
	movUsersData.push_back(oNewUser);
	mvDataToComboBox();
}


// **************************************************************************
void
ManagePlaylists_c::mvDelUser(int aiIndex)
{
	if( aiIndex>=0 && aiIndex<int(movUsersData.size()) ) {
		movUsersData.erase(  movUsersData.begin()+aiIndex  );
		mvDataToComboBox();
	}
}


// **************************************************************************
void
ManagePlaylists_c::mvAddPlaylist(const QString& aorName)
{
	if(miCurrentUserIndex>=0) {
		UsersData_s::Playlists_s oNewPlaylist ;
		oNewPlaylist.sName = aorName;
		movUsersData[uint32_t(miCurrentUserIndex)].ovPlaylists.push_back(oNewPlaylist);
		mvDataToPlaylistTable();
	}
}


// **************************************************************************
void
ManagePlaylists_c::mvDelPlaylist(int aiIndex)
{
	if(miCurrentUserIndex>=0  && miCurrentUserIndex<int(movUsersData.size()) ) {
		auto& orPL = movUsersData[uint32_t(miCurrentUserIndex)].ovPlaylists;
		if(aiIndex>=0 && aiIndex<int(orPL.size())) {
			orPL.erase(orPL.begin()+aiIndex);
			mvDataToPlaylistTable();
		}
	}
}


// **************************************************************************
void
ManagePlaylists_c::mvAddTrack(UsersData_s::Playlists_s::Track_s& aorTrack)
{
	if(miCurrentPlaylistIndex >=0 && miCurrentUserIndex >=0) {
		auto& orTracks = movUsersData[ uint32_t(miCurrentUserIndex) ].ovPlaylists[ uint32_t(miCurrentPlaylistIndex) ].ovTracks ;
		orTracks.push_back(aorTrack);
		mvDataToTracksTable();
	}
}


// **************************************************************************
void
ManagePlaylists_c::mvDelTrack(int aiIndex)
{
	if(miCurrentUserIndex>=0  && miCurrentUserIndex<int(movUsersData.size()) ) {
		auto& orPL = movUsersData[ uint32_t(miCurrentUserIndex) ].ovPlaylists;

		if(miCurrentPlaylistIndex>=0 && miCurrentPlaylistIndex<int(orPL.size())){
			auto& orTrk = orPL[ uint32_t(miCurrentPlaylistIndex) ].ovTracks;

			orTrk.erase(orTrk.begin()+aiIndex);
			mvDataToTracksTable();
		}
	}
}


// **************************************************************************
void
ManagePlaylists_c::mvSetWidgets(QTableWidget *aopW_TablePlaylists, QTableWidget* aopW_TableTracks, QComboBox* aopW_ComboBoxUsers)
{
	mopW_TablePlaylists = aopW_TablePlaylists ;
	mopW_TableTracks    = aopW_TableTracks;
	mopW_ComboBoxUsers  = aopW_ComboBoxUsers ;
}


// **************************************************************************
void
ManagePlaylists_c::mvDataToPlaylistTable()
{
	if(nullptr != mopW_TablePlaylists) {
		mopW_TablePlaylists->setRowCount(0); // Erase All

		if( movUsersData.size() != 0  &&  miCurrentUserIndex >=0 ) {
			auto& orPL = movUsersData[size_t(miCurrentUserIndex)].ovPlaylists;

			// Fill the table
			for(uint16_t uPL=0 ; uPL<orPL.size() ; uPL++) {
				mopW_TablePlaylists->insertRow(uPL);
				// Nao ha vazamento de memoria: QTableWidget apagara os QTableWidgetItem
				QString sStr = orPL[uPL].sName;
				QTableWidgetItem* opName = new QTableWidgetItem(sStr);

				// Itens nao-editaveis:
				opName->setFlags( opName->flags() &~Qt::ItemIsEditable );
				mopW_TablePlaylists->setItem(uPL, 0, opName);
			}
			mopW_TablePlaylists->repaint();
		}
	}
}


// **************************************************************************
void
ManagePlaylists_c::mvDataToTracksTable()
{
	if(miCurrentPlaylistIndex >=0 && miCurrentUserIndex >=0) {
		mopW_TableTracks->setRowCount(0); // Erase All

		if( movUsersData.size() != 0) {
			auto& orPL = movUsersData[size_t(miCurrentUserIndex)].ovPlaylists;

			if(orPL.size() >0) {
				auto& orTracks = orPL[size_t(miCurrentPlaylistIndex)].ovTracks;


				// Fill the table
				for(uint16_t uTr=0 ; uTr<orTracks.size() ; uTr++) {
					mopW_TableTracks->insertRow(uTr);
					// Nao ha vazamento de memoria: QTableWidget apagara os QTableWidgetItem
					QString sTrack  = orTracks[uTr].sTrackName ;
					QString sArtist = orTracks[uTr].sArtist ;
					QString sAlbum  = orTracks[uTr].sAlbumName ;
					QString sDate   = orTracks[uTr].sReleaseDate ;

					QTableWidgetItem* opTrack  = new QTableWidgetItem(sTrack );
					QTableWidgetItem* opArtist = new QTableWidgetItem(sArtist);
					QTableWidgetItem* opAlbum  = new QTableWidgetItem(sAlbum );
					QTableWidgetItem* opDate   = new QTableWidgetItem(sDate  );

					// Itens nao-editaveis:
					opTrack ->setFlags( opTrack ->flags() &~Qt::ItemIsEditable );
					opArtist->setFlags( opArtist->flags() &~Qt::ItemIsEditable );
					opAlbum ->setFlags( opAlbum ->flags() &~Qt::ItemIsEditable );
					opDate  ->setFlags( opDate  ->flags() &~Qt::ItemIsEditable );

					mopW_TableTracks->setItem(uTr, 0, opTrack );
					mopW_TableTracks->setItem(uTr, 1, opArtist);
					mopW_TableTracks->setItem(uTr, 2, opAlbum );
					mopW_TableTracks->setItem(uTr, 3, opDate  );
				}
				mopW_TableTracks->repaint(); // Verificar necessidade
			}
		}
	}
}


// **************************************************************************
void
ManagePlaylists_c::mvDataToComboBox()
{
	if(nullptr != mopW_ComboBoxUsers) {
		mopW_ComboBoxUsers->clear();

		int i = mopW_ComboBoxUsers->currentIndex();
		for(uint16_t u=0 ; u<movUsersData.size() ; u++){
			mopW_ComboBoxUsers->addItem(movUsersData[u].sName);
		}
		mopW_ComboBoxUsers->setCurrentIndex(i); // no seletcion
	}
}


// **************************************************************************
ManagePlaylists_c::UsersData_s::Playlists_s::Track_s
ManagePlaylists_c::moGetDataTrack(int aiTrackIndex)
{
	UsersData_s::Playlists_s::Track_s oRet;

	if(miCurrentPlaylistIndex >=0 && miCurrentUserIndex >=0) {
		oRet = movUsersData[ uint32_t(miCurrentUserIndex) ].ovPlaylists[ uint32_t(miCurrentPlaylistIndex) ].ovTracks[ uint32_t(aiTrackIndex) ];
	}
	return oRet;
}


// **************************************************************************
void
ManagePlaylists_c::mvSetActiveUser(int aiIndex)
{
	if( aiIndex < int(movUsersData.size()) ) {
		if(miCurrentUserIndex != aiIndex) {
			miCurrentUserIndex = aiIndex;
			mvDataToPlaylistTable();
			mopW_TableTracks->setRowCount(0); // Erase All

		}
	} else {
		miCurrentUserIndex = -1; // sem selecao
	}
}


// **************************************************************************
void
ManagePlaylists_c::mvSetActivePlaylist(int aiIndex)
{
	if( movUsersData.size() > 0) {
		if(miCurrentUserIndex != -1) {
			miCurrentPlaylistIndex = aiIndex;
			mvDataToTracksTable();
		}
	}

}




