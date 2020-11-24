#include "mainwindow.h"
#include "ui_mainwindow.h"

// Prj
#include "ManagePlaylists_c.h"
#include "SpotifyAPI/SpotifyUserSecrets_c.h"
#include "MediaPlayer_c.h"

// C/C++
#include <vector>

// Qt


// ****************************************************************************
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, moAudioPlayer(this)
{
	ui->setupUi(this);

	setFixedSize(size()); // Nao permite redimensionamento

	// Configura tabela Search
	{
		ui->mopW_TableWidget_Search->insertColumn(0);
		ui->mopW_TableWidget_Search->insertColumn(1);
		ui->mopW_TableWidget_Search->insertColumn(2);
		ui->mopW_TableWidget_Search->insertColumn(3);
//		ui->mopW_TableWidget_Search->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		ui->mopW_TableWidget_Search->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui->mopW_TableWidget_Search->resizeColumnToContents(1);

		QStringList svList = { "Música", "Artista", "Álbum","Data Lançamento"} ;
		ui->mopW_TableWidget_Search->setHorizontalHeaderLabels(svList);

		ui->mopW_TableWidget_Search->setColumnWidth(0, 220);
		ui->mopW_TableWidget_Search->setColumnWidth(1, 220);
	}

	// Configura tabela Tracks
	{
		ui->mopW_TableWidget_Tracks->insertColumn(0);
		ui->mopW_TableWidget_Tracks->insertColumn(1);
		ui->mopW_TableWidget_Tracks->insertColumn(2);
		ui->mopW_TableWidget_Tracks->insertColumn(3);
		ui->mopW_TableWidget_Tracks->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui->mopW_TableWidget_Tracks->resizeColumnToContents(1);

		QStringList svList = { "Música", "Artista", "Álbum","Data Lançamento"} ;
		ui->mopW_TableWidget_Tracks->setHorizontalHeaderLabels(svList);
	}


	// Configura tabela Playlists
	{
		ui->mopW_TableWidget_Playlists->insertColumn(0);
		ui->mopW_TableWidget_Playlists->insertColumn(1);
//		ui->mopW_TableWidget_Playlists->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui->mopW_TableWidget_Playlists->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		ui->mopW_TableWidget_Playlists->resizeColumnToContents(1);

		QStringList svList = { "Nome", "Data"} ;
		ui->mopW_TableWidget_Playlists->setHorizontalHeaderLabels(svList);

		ui->mopW_TableWidget_Playlists->setColumnWidth(0, 150);
		ui->mopW_TableWidget_Playlists->setColumnWidth(1, 50);
	}

	// Alguns botoes desligados:
	ui->mopW_PushButton_AdicionarTrack->setEnabled( false );
	ui->mopW_PushButton_AddPlaylist->setEnabled(false);

	ui->mopW_PushButton_AddUser->setEnabled(false);
	ui->mopW_PushButton_DelUser->setEnabled(false);
	ui->mopW_LineEdit_NewPlaylist->setEnabled(false);


	// Volume:
	ui->mopW_VerticalSlider_Volume->setMaximum(100);
	ui->mopW_VerticalSlider_Volume->setValue(25);


	moManagePlaylists.mvSetWidgets(
				ui->mopW_TableWidget_Playlists,
				ui->mopW_TableWidget_Tracks,
				ui->mopW_ComboBox_Users);
	moManagePlaylists.mvFromFile() ;
}


// ****************************************************************************
MainWindow::~MainWindow()
{
	moManagePlaylists.mvToFile(); // Salva todos dados no drive.
	delete ui;
}


// ****************************************************************************
void
MainWindow::on_mopW_PushButton_ConfigClienteSpotify_clicked()
{
	// Abre nova tela de Dialogo
	// https://www.youtube.com/watch?v=tP70B-pdTH0&ab_channel=ProgrammingKnowledge
	SpotifyUserSecrets_c oSpotifyUserSecrets;

	DialogClient_d oDialogClient(oSpotifyUserSecrets, this);
	oDialogClient.setModal(true);
	oDialogClient.exec();

	uint32_t uTimeout_ms = 3500 ;
	uint32_t uRespTime_ms = moSpotifyAPI.mvSetConnection(oSpotifyUserSecrets, uTimeout_ms);


	ui->mopW_TextBrowser_Out->clear();
	ui->mopW_TextBrowser_Out->setText("Tempo total de acesso: " + QString::number(uRespTime_ms/1000.0) + 's');

	SpotifyAPI_c::ConnectionStatus_e eStatus = moSpotifyAPI.meGetConnectionStatus() ;
	ui->mopW_TextBrowser_Out->append("\nCódigo de erro: " + QString::number(int(eStatus)));

	QString sError = moSpotifyAPI.msGetLastError() ;
	ui->mopW_TextBrowser_Out->append("\n" + sError);
}


// ****************************************************************************
// Bora procurar
void MainWindow::on_mopW_PushButton_Search_clicked()
{
	// Clear Table
	ui->mopW_TableWidget_Search->setRowCount(0);

	ui->mopW_PushButton_Search->setEnabled(false);
	ui->mopW_TableWidget_Search->setEnabled(false);
	ui->mopW_LineEdit_Search->setEnabled(false);


	movSearchResult.clear();
	QString sSentence = ui->mopW_LineEdit_Search->text();

	uint32_t uNumTracks = ui->mopW_LineEdit_MaxTracks->text().toUInt();

	bool bSetFlag = ui->mopW_CheckBox_30s->isChecked();
	uint32_t uSearchFlags = bSetFlag? uint32_t(SpotifyAPI_c::Flags_e::eIS_PLAYABLE_30s) : 0;

	uint32_t uAccessTime_ms = moSpotifyAPI.movSearchTrack(
				sSentence,
				uNumTracks,
				uSearchFlags,
				movSearchResult);

	ui->mopW_TextBrowser_Out->clear();
	ui->mopW_TextBrowser_Out->setText("Tempo total de acesso: "+QString::number(uAccessTime_ms/1000.0) + 's');

	SpotifyAPI_c::ConnectionStatus_e eStatus = moSpotifyAPI.meGetConnectionStatus() ;
	ui->mopW_TextBrowser_Out->append("\nCódigo de erro: " + QString::number(int(eStatus)));

	QString sError = moSpotifyAPI.msGetLastError() ;
	ui->mopW_TextBrowser_Out->append("\n" + sError);

	// Populate Data
	for(uint16_t u=0 ; u<movSearchResult.size() ; u++) {
		ui->mopW_TableWidget_Search->insertRow(u);

		// Nao ha vazamento de memoria: QTableWidget apagara os QTableWidgetItem
		QTableWidgetItem* opName   = new QTableWidgetItem(movSearchResult[u].sName);
		QTableWidgetItem* opArtist = new QTableWidgetItem(movSearchResult[u].ovArtists[0].sName);
		QTableWidgetItem* opAlbum  = new QTableWidgetItem(movSearchResult[u].oAlbum.sName);
		QTableWidgetItem* opDate   = new QTableWidgetItem(movSearchResult[u].oAlbum.sReleaseDate);

		// Itens nao-editaveis:
		opName->setFlags  ( opName->flags()   &~Qt::ItemIsEditable );
		opArtist->setFlags( opArtist->flags() &~Qt::ItemIsEditable );
		opAlbum->setFlags ( opAlbum->flags()  &~Qt::ItemIsEditable );
		opDate->setFlags  ( opDate->flags()   &~Qt::ItemIsEditable );
		ui->mopW_TableWidget_Search->setItem(u, 0, opName);
		ui->mopW_TableWidget_Search->setItem(u, 1, opArtist);
		ui->mopW_TableWidget_Search->setItem(u, 2, opAlbum);
		ui->mopW_TableWidget_Search->setItem(u, 3, opDate);

	}

	ui->mopW_TableWidget_Search->repaint();
	ui->mopW_PushButton_Search->setEnabled(true);
	ui->mopW_TableWidget_Search->setEnabled(true);
	ui->mopW_LineEdit_Search->setEnabled(true);
}


// ****************************************************************************
// Seleciona um dos usuarios!
void
MainWindow::on_mopW_ComboBox_Users_currentIndexChanged(int aiIndex)
{
	moManagePlaylists.mvSetActiveUser(aiIndex);
	bool bEnable = (aiIndex>=0);  //Nenhum item selecionado
	ui->mopW_PushButton_DelUser->setEnabled(bEnable);
	ui->mopW_LineEdit_NewPlaylist->setEnabled(bEnable);
}


// ****************************************************************************
// Insere musica na playlist!
void MainWindow::on_mopW_PushButton_AdicionarTrack_clicked()
{
	ManagePlaylists_c::UsersData_s::Playlists_s::Track_s oTrack;

	uint32_t uPos = uint32_t(  ui->mopW_TableWidget_Search->currentRow()  );
	oTrack.sTrackName   = movSearchResult[uPos].sName ;
	oTrack.sAlbumName   = movSearchResult[uPos].oAlbum.sName;
	oTrack.sReleaseDate = movSearchResult[uPos].oAlbum.sReleaseDate;
	oTrack.sPreviewURL  = movSearchResult[uPos].sPreview_url;

	if(movSearchResult[uPos].ovArtists.size() !=0) {
		oTrack.sArtist      = movSearchResult[uPos].ovArtists[0].sName; // Sim, pega so o primeiro
	}

	if(movSearchResult[uPos].oAlbum.ovImages.size() !=0) {
		oTrack.sImage       = movSearchResult[uPos].oAlbum.ovImages[0].sUrl;
	}

	moManagePlaylists.mvAddTrack(oTrack);
}


// **************************************************************************
//
void MainWindow::on_mopW_PushButton_AddUser_clicked()
{
	QString sName = ui->mopW_LineEdit_NewUser->text();
	moManagePlaylists.mvAddUser(sName);
	ui->mopW_LineEdit_NewUser->setText("");
}


// **************************************************************************
//
void
MainWindow::on_mopW_PushButton_DelUser_clicked()
{
	int iIndex = ui->mopW_ComboBox_Users->currentIndex();
	moManagePlaylists.mvDelUser(iIndex);
}


// **************************************************************************
void
MainWindow::on_mopW_LineEdit_NewUser_textChanged(const QString& asrNewUser)
{
	bool bDuplicate = false ;
	for(uint16_t u=0 ; u<ui->mopW_ComboBox_Users->count() ; u++) {
		bDuplicate |= (asrNewUser == ui->mopW_ComboBox_Users->itemText(u));
	}

	bool bEnableAdd = (asrNewUser!="") && !bDuplicate ;
	ui->mopW_PushButton_AddUser->setEnabled(bEnableAdd);
}


// **************************************************************************
void
MainWindow::on_mopW_PushButton_AddPlaylist_clicked()
{
	QString sNewPlaylist = ui->mopW_LineEdit_NewPlaylist->text();
	moManagePlaylists.mvAddPlaylist(sNewPlaylist);
	ui->mopW_LineEdit_NewPlaylist->setText("");
}


// **************************************************************************
void MainWindow::on_mopW_PushButton_DelPlaylist_clicked()
{
	int aiIndex = ui->mopW_TableWidget_Playlists->currentRow();
	moManagePlaylists.mvDelPlaylist(aiIndex) ;
}


// **************************************************************************
void
MainWindow::on_mopW_LineEdit_NewPlaylist_textChanged(const QString &asrNewPlaylist)
{
	//mopW_LineEdit_NewPlaylist
	bool bDuplicate = false ;
	for(uint16_t u=0 ; u<ui->mopW_TableWidget_Playlists->rowCount() ; u++) {
		QString sItemText = ui->mopW_TableWidget_Playlists->item(u,0)->text();
		bDuplicate |= (asrNewPlaylist == sItemText);
	}

	bool bEnableAdd = (asrNewPlaylist!="") && !bDuplicate ;
	ui->mopW_PushButton_AddPlaylist->setEnabled(bEnableAdd);
}


// **************************************************************************
void
MainWindow::on_mopW_PushButton_DelTrack_clicked()
{
	int iIndex = ui->mopW_TableWidget_Tracks->currentRow();
	moManagePlaylists.mvDelTrack(iIndex);
}





// **************************************************************************
void
MainWindow::on_mopW_PushButton_PlayAudio_clicked()
{

	moAudioPlayer.mvPlay();
	// ▐ ▌■
}


// **************************************************************************
void
MainWindow::on_mopW_PushButton_PauseAudio_clicked()
{
	moAudioPlayer.mvPause();
}


// **************************************************************************
void MainWindow::on_mopW_PushButton_StopAudio_clicked()
{
	moAudioPlayer.mvStop();
}










// ****************************************************************************
void
MainWindow::on_mopW_TableWidget_Search_cellPressed(int , int)
{
	mvSignalsTableWidget_Search();
}


// ****************************************************************************
void
MainWindow::on_mopW_TableWidget_Search_cellChanged(int aiRow, int aiColumn)
{
	// testa apenas elemento (zero,zero), por tabela modificada
	// Diminui overhead
	if(0==aiRow  &&  0==aiColumn) {
		mvSignalsTableWidget_Search();
	}
}


// **************************************************************************
void
MainWindow::mvSignalsTableWidget_Search()
{
	int iRow = ui->mopW_TableWidget_Search->currentRow();

	mvBtnAdd_Test();

	moAudioPlayer.mvStop();
	if(iRow!=-1) {
		auto& oTrackInfo = movSearchResult[ uint32_t(iRow) ];

		if(oTrackInfo.sPreview_url != "") {
			MediaPlayer_c::PlaylistItem_s oItem ;
			std::vector<MediaPlayer_c::PlaylistItem_s> ovPlaylistTemp ;

			oItem.sName     = oTrackInfo.sName;
			oItem.sAlbum    = oTrackInfo.oAlbum.sName;
			oItem.sPlayLink = oTrackInfo.sPreview_url;

			ovPlaylistTemp.push_back(oItem);

			moAudioPlayer.mvSetPlaylist(ovPlaylistTemp);
		}
	}
}








// ****************************************************************************
void MainWindow::on_mopW_TableWidget_Playlists_cellPressed(int , int)
// Seleciona uma das playlists!
{
	mvSignalsTableWidget_Playlists();
}


// ****************************************************************************
void
MainWindow::on_mopW_TableWidget_Playlists_cellChanged(int aiRow, int aiColumn)
{
	// testa apenas elemento (zero,zero), por tabela modificada
	// Diminui overhead
	if(0==aiRow  &&  0==aiColumn) {
		mvSignalsTableWidget_Playlists();
	}
}


// **************************************************************************
void
MainWindow::mvSignalsTableWidget_Playlists()
{
	int iRow = ui->mopW_TableWidget_Playlists->currentRow();

	moManagePlaylists.mvSetActivePlaylist(iRow);

	mvBtnAdd_Test();

	moAudioPlayer.mvStop();
	if(iRow!=-1) {
		ManagePlaylists_c::UsersData_s::Playlists_s::Track_s oTrackInfo;

		std::vector<MediaPlayer_c::PlaylistItem_s> ovPlaylistTemp ;
		int iSize = ui->mopW_TableWidget_Tracks->rowCount() ;
		for(int i=0 ; i<iSize ; i++) {
			MediaPlayer_c::PlaylistItem_s oItem ;
			auto oItemTrackList = moManagePlaylists.moGetDataTrack(i);

			oItem.sName     = oItemTrackList.sTrackName;
			oItem.sAlbum    = oItemTrackList.sAlbumName;
			oItem.sPlayLink = oItemTrackList.sPreviewURL;

			ovPlaylistTemp.push_back(oItem);
		}
		moAudioPlayer.mvSetPlaylist(ovPlaylistTemp);

	}
}







void MainWindow::on_mopW_TableWidget_Tracks_cellPressed(int, int)
{
	mvSignalsTableWidget_Tracks();
}


// **************************************************************************
void MainWindow::on_mopW_TableWidget_Tracks_cellChanged(int aiRow, int aiColumn)
{
	// testa apenas elemento (zero,zero), por tabela modificada
	// Diminui overhead
	if(0==aiRow  &&  0==aiColumn) {
		mvSignalsTableWidget_Tracks();
	}
}


// **************************************************************************
void
MainWindow::mvSignalsTableWidget_Tracks()
{
	int iRow = ui->mopW_TableWidget_Tracks->currentRow();

	bool bRowExist = iRow!=-1;
	ui->mopW_PushButton_DelTrack->setEnabled(bRowExist);

	mvBtnAdd_Test();

	moAudioPlayer.mvStop();
	if(bRowExist) {
		ManagePlaylists_c::UsersData_s::Playlists_s::Track_s oTrackInfo;
		oTrackInfo = moManagePlaylists.moGetDataTrack(iRow);
		QString sLink = oTrackInfo.sPreviewURL ;

		if(sLink!="") {
			MediaPlayer_c::PlaylistItem_s oItem ;
			std::vector<MediaPlayer_c::PlaylistItem_s> ovPlaylistTemp ;

			auto oItemTrackList = moManagePlaylists.moGetDataTrack(iRow);

			oItem.sName     = oItemTrackList.sTrackName;
			oItem.sAlbum    = oItemTrackList.sAlbumName;
			oItem.sPlayLink = oItemTrackList.sPreviewURL;

			ovPlaylistTemp.push_back(oItem);

			moAudioPlayer.mvSetPlaylist(ovPlaylistTemp);
		}
	}
}






// **************************************************************************
void
MainWindow::mvBtnAdd_Test()
{
	int iRowPlaylists = ui->mopW_TableWidget_Playlists->currentRow();
	int iRowSearch    = ui->mopW_TableWidget_Search->currentRow();

	bool bLigarBtn = (iRowPlaylists >= 0) && (iRowSearch >= 0);
	ui->mopW_PushButton_AdicionarTrack->setEnabled( bLigarBtn );
}


// **************************************************************************
void
MainWindow::on_mopW_VerticalSlider_Volume_valueChanged(int aiValue)
{
	float fVolume = float(aiValue) / float(ui->mopW_VerticalSlider_Volume->maximum());

	moAudioPlayer.mvSetVolume(fVolume) ;
}
