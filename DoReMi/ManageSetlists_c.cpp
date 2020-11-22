
// Prj
#include "ManageSetlists_c.h"

// C/C++
#include <vector>

// Qt
#include <QComboBox>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


const char ManageSetlists_c::msCryptoTest[] ="123";


// **************************************************************************
ManageSetlists_c::ManageSetlists_c()
{
//	mvFromFile();
//	mvToFile();
}


// **************************************************************************
void
ManageSetlists_c::mvFromFile()
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

			QJsonArray oArraySetLists = oJsonElem["vSetLists"].toArray();
			foreach(const QJsonValue& oJsonElemSetList, oArraySetLists) {
				UsersData_s::SetLists_s oSetList ;
				oSetList.sName = oJsonElemSetList["sName"].toString();

				QJsonArray oArrayTracks = oJsonElemSetList["vTracks"].toArray();
				foreach(const QJsonValue& oJsonElemTrack, oArrayTracks) {
					UsersData_s::SetLists_s::Track_s oTrack ;
					oTrack.sTrackName   = oJsonElemTrack["sTrackName"  ].toString();
					oTrack.sAlbumName   = oJsonElemTrack["sAlbumName"  ].toString();
					oTrack.sArtist      = oJsonElemTrack["sArtist"     ].toString();
					oTrack.sImage       = oJsonElemTrack["sImage"      ].toString();
					oTrack.sPreviewURL  = oJsonElemTrack["sPreviewURL" ].toString();
					oTrack.sReleaseDate = oJsonElemTrack["sReleaseDate"].toString();

					oSetList.ovTracks.push_back(oTrack);
				}
				oUser.ovSetLists.push_back(oSetList);
			}
			movUsersData.push_back(oUser) ;
		}
	}

	// pra tela
	mvDataToSetlistTable() ;
	mvDataToComboBox();
}


// **************************************************************************
void
ManageSetlists_c::mvToFile()
{
	QJsonArray oJsonArrayUsers ;
	for(uint16_t uUsr=0 ; uUsr<movUsersData.size() ; uUsr++) { // sim, eu sei que da pra usar iterator.
		QJsonObject oJsonUserData ;
		oJsonUserData["sName"] = movUsersData[uUsr].sName;

		QJsonArray oJsonArraySetlists ;
		for(uint16_t uSl=0 ; uSl<movUsersData[uUsr].ovSetLists.size(); uSl++) {
			QJsonObject oJsonSetList ;
			oJsonSetList["sName"] = movUsersData[uUsr].ovSetLists[uSl].sName;

			QJsonArray oJsonArrayTracks ;
			for(uint16_t uTr=0 ; uTr<movUsersData[uUsr].ovSetLists[uSl].ovTracks.size(); uTr++) {
				QJsonObject oJsonTrackData ;
				oJsonTrackData["sTrackName"  ] = movUsersData[uUsr].ovSetLists[uSl].ovTracks[uTr].sTrackName;
				oJsonTrackData["sAlbumName"  ] = movUsersData[uUsr].ovSetLists[uSl].ovTracks[uTr].sAlbumName;
				oJsonTrackData["sArtist"     ] = movUsersData[uUsr].ovSetLists[uSl].ovTracks[uTr].sArtist;
				oJsonTrackData["sImage"      ] = movUsersData[uUsr].ovSetLists[uSl].ovTracks[uTr].sImage;
				oJsonTrackData["sPreviewURL" ] = movUsersData[uUsr].ovSetLists[uSl].ovTracks[uTr].sPreviewURL;
				oJsonTrackData["sReleaseDate"] = movUsersData[uUsr].ovSetLists[uSl].ovTracks[uTr].sReleaseDate;

				oJsonArrayTracks.append(oJsonTrackData);
				//qMakePair(QString("sClient_ID"    ), QJsonValue(QString(oElem.sClient_ID    ))),
			}


			oJsonSetList["vTracks"] = oJsonArrayTracks;
			oJsonArraySetlists.append(oJsonSetList);

		}

		oJsonUserData["vSetLists"] = oJsonArraySetlists;
		oJsonArrayUsers.append(oJsonUserData);
	}

	QByteArray sRawDataFile = QJsonDocument(oJsonArrayUsers).toJson() ;

	QFile sDataFile( QString("S_")+msFileName ) ;
	sDataFile.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Unbuffered ) ;
	sDataFile.write( sRawDataFile ) ;
	sDataFile.close() ;
}


// **************************************************************************
void
ManageSetlists_c::mvAddElement(UsersData_s& aorUserData)
{
	movUsersData.push_back(aorUserData) ;
	mvDataToSetlistTable() ;
}


// **************************************************************************
void
ManageSetlists_c::mvDelElement(int aiIndex)
{
	if(aiIndex >=0  && aiIndex < int(movUsersData.size())) {
		auto it = movUsersData.begin();
		movUsersData.erase(it);
	}
	mvDataToSetlistTable() ;
	mvDataToComboBox();
}

/*
// **************************************************************************
bool
ManageSetlists_c::mbSetOrder(bool abCrescent)
{

}
*/

// **************************************************************************
void
ManageSetlists_c::mvSetWidgets(QTableWidget *aopW_TableSetlists, QTableWidget* aopW_TableTracks, QComboBox* aopW_ComboBoxUsers)
{
	mopW_TableSetlists = aopW_TableSetlists ;
	mopW_TableTracks   = aopW_TableTracks;
	mopW_ComboBoxUsers = aopW_ComboBoxUsers ;
}


// **************************************************************************
void
ManageSetlists_c::mvDataToSetlistTable()
{
	if(nullptr != mopW_TableSetlists) {
		mopW_TableSetlists->setRowCount(0); // Erase All

		if( movUsersData.size() != 0  &&  miCurrentUserIndex >=0 ) {
			auto& orSL = movUsersData[size_t(miCurrentUserIndex)].ovSetLists;

			// Fill the table
			for(uint16_t uSL=0 ; uSL<orSL.size() ; uSL++) {
				mopW_TableSetlists->insertRow(uSL);
				// Nao ha vazamento de memoria: QTableWidget apagara os QTableWidgetItem
				QString sStr = orSL[uSL].sName;
				QTableWidgetItem* opName = new QTableWidgetItem(sStr);

				// Itens nao-editaveis:
				opName->setFlags( opName->flags() &~Qt::ItemIsEditable );
				mopW_TableSetlists->setItem(uSL, 0, opName);
			}
			mopW_TableSetlists->repaint();
		}
	}
}


// **************************************************************************
void
ManageSetlists_c::mvDataToTracksTable()
{
	if(miCurrentSetlistIndex >=0 && miCurrentUserIndex >=0) {
		mopW_TableTracks->setRowCount(0); // Erase All

		if( movUsersData.size() != 0) {
			auto& orSL = movUsersData[size_t(miCurrentUserIndex)].ovSetLists;

			if(orSL.size() >0) {
				auto& orTracks = orSL[size_t(miCurrentSetlistIndex)].ovTracks;


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
ManageSetlists_c::mvDataToComboBox()
{
	if(nullptr != mopW_ComboBoxUsers) {
		int i = mopW_ComboBoxUsers->currentIndex();
		for(uint16_t u=0 ; u<movUsersData.size() ; u++){

			mopW_ComboBoxUsers->addItem(movUsersData[u].sName);
		}
		mopW_ComboBoxUsers->setCurrentIndex(i); // no seletcion
	}
}


// **************************************************************************
ManageSetlists_c::UsersData_s
ManageSetlists_c::moGetDataItem(uint16_t auItem)
{
	UsersData_s oRet;
	if(auItem < movUsersData.size()) {
		oRet = movUsersData[auItem];
	}
	return oRet;
}


// **************************************************************************
void
ManageSetlists_c::mvSetActiveUser(int aiIndex)
{
	if( aiIndex < int(movUsersData.size()) ) {
		miCurrentUserIndex = aiIndex;
		mvDataToSetlistTable();
	} else {
		miCurrentUserIndex = -1; // sem selecao
	}
}


// **************************************************************************
void
ManageSetlists_c::mvSetActiveSetlist(int aiIndex)
{
	if( movUsersData.size() > 0) {
		if(miCurrentUserIndex != -1) {
			miCurrentSetlistIndex = aiIndex;
			mvDataToTracksTable();
		}
	}

}


