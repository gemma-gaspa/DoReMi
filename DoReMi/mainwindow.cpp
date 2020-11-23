#include "mainwindow.h"
#include "ui_mainwindow.h"

// Prj
#include "SpotifyAPI/SpotifyUserSecrets_c.h"

// Qt
#include <QNetworkAccessManager>
#include <QNetworkReply>


// ****************************************************************************
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	// Configura tabela Search
	{
		ui->mopW_TableWidget_Search->insertColumn(0);
		ui->mopW_TableWidget_Search->insertColumn(1);
		ui->mopW_TableWidget_Search->insertColumn(2);
		ui->mopW_TableWidget_Search->insertColumn(3);
		//ui->mopW_TableWidget_Search->setResizeMode(QHeaderView::ResizeToContents);
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
		//ui->mopW_TableWidget_Tracks->setResizeMode(QHeaderView::ResizeToContents);
		ui->mopW_TableWidget_Tracks->resizeColumnToContents(1);

		QStringList svList = { "Música", "Artista", "Álbum","Data Lançamento"} ;
		ui->mopW_TableWidget_Tracks->setHorizontalHeaderLabels(svList);
	}


	// Configura tabela Playlists
	{
		ui->mopW_TableWidget_Playlists->insertColumn(0);
		ui->mopW_TableWidget_Playlists->insertColumn(1);
		//ui->mopW_TableWidget_Playlists->setResizeMode(QHeaderView::ResizeToContents);
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






	//moManageSetLists
	moManageSetLists.mvSetWidgets(
				ui->mopW_TableWidget_Playlists,
				ui->mopW_TableWidget_Tracks,
				ui->mopW_ComboBox_Users);
	moManageSetLists.mvFromFile() ;
}


// ****************************************************************************
MainWindow::~MainWindow()
{
	moManageSetLists.mvToFile(); // Salva todos dados no drive.
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

	uint32_t uDelay_ms = 3500 ;
	uint32_t uRespTime_ms = moSpotifyAPI.mvSetConnection(oSpotifyUserSecrets, uDelay_ms);
	uRespTime_ms;
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


	movResult.clear();
	QString sSentence = ui->mopW_LineEdit_Search->text();

	uint32_t uNumTracks = ui->mopW_LineEdit_MaxTracks->text().toUInt();

	bool bSetFlag = ui->mopW_CheckBox_30s->isChecked();
	uint32_t uSearchFlags = bSetFlag? uint32_t(SpotifyAPI_c::Flags_e::eIS_PLAYABLE_30s) : 0;

	uint32_t uAccessTime_ms = moSpotifyAPI.movSearchTrack(
				sSentence,
				uNumTracks,
				uSearchFlags,
				movResult);

	ui->mopW_TextBrowser_Out->setText("Tempo total de acesso: "+QString::number(uAccessTime_ms/1000.0));


	// Populate Data
	for(uint16_t u=0 ; u<movResult.size() ; u++) {
		ui->mopW_TableWidget_Search->insertRow(u);

		// Nao ha vazamento de memoria: QTableWidget apagara os QTableWidgetItem
		QTableWidgetItem* opName   = new QTableWidgetItem(movResult[u].sName);
		QTableWidgetItem* opArtist = new QTableWidgetItem(movResult[u].ovArtists[0].sName);
		QTableWidgetItem* opAlbum  = new QTableWidgetItem(movResult[u].oAlbum.sName);
		QTableWidgetItem* opDate   = new QTableWidgetItem(movResult[u].oAlbum.sReleaseDate);

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
	moManageSetLists.mvSetActiveUser(aiIndex);
	bool bEnable = (aiIndex>=0);  //Nenhum item selecionado
	ui->mopW_PushButton_DelUser->setEnabled(bEnable);
	ui->mopW_LineEdit_NewPlaylist->setEnabled(bEnable);
}


// ****************************************************************************
// Seleciona uma das playlists!
void
MainWindow::on_mopW_TableWidget_Playlists_currentCellChanged(
		int currentRow,
		int /*currentColumn*/,
		int previousRow,
		int /*previousColumn*/)
{
	if(currentRow != previousRow) {
		moManageSetLists.mvSetActiveSetlist(currentRow);
	}
}


// ****************************************************************************
// Ativa Botao de Inserir Musica
void MainWindow::on_mopW_TableWidget_Search_currentCellChanged(
		int currentRow,
		int /*currentColumn*/,
		int /*previousRow*/,
		int /*previousColumn*/)
{
	bool bLigarBtn = (currentRow != -1) && (ui->mopW_TableWidget_Playlists->currentRow() >=0);
	ui->mopW_PushButton_AdicionarTrack->setEnabled( bLigarBtn );
}


// ****************************************************************************
// Insere musica na setlist!
void MainWindow::on_mopW_PushButton_AdicionarTrack_clicked()
{
	ManageSetlists_c::UsersData_s::SetLists_s::Track_s oTrack;

	uint32_t uPos = uint32_t(  ui->mopW_TableWidget_Search->currentRow()  );
	oTrack.sTrackName   = movResult[uPos].sName ;
	oTrack.sAlbumName   = movResult[uPos].oAlbum.sName;
	oTrack.sReleaseDate = movResult[uPos].oAlbum.sReleaseDate;
	oTrack.sPreviewURL  = movResult[uPos].sPreview_url;

	if(movResult[uPos].ovArtists.size() !=0) {
		oTrack.sArtist      = movResult[uPos].ovArtists[0].sName; // Sim, pega so o primeiro
	}

	if(movResult[uPos].oAlbum.ovImages.size() !=0) {
		oTrack.sImage       = movResult[uPos].oAlbum.ovImages[0].sUrl;
	}

	moManageSetLists.mvAddTrack(oTrack);
}


// **************************************************************************
//
void MainWindow::on_mopW_PushButton_AddUser_clicked()
{
	QString sName = ui->mopW_LineEdit_NewUser->text();
	moManageSetLists.mvAddUser(sName);
	ui->mopW_LineEdit_NewUser->setText("");
}


// **************************************************************************
//
void
MainWindow::on_mopW_PushButton_DelUser_clicked()
{
	int iIndex = ui->mopW_ComboBox_Users->currentIndex();
	moManageSetLists.mvDelUser(iIndex);
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
	moManageSetLists.mvAddSetlist(sNewPlaylist);
	ui->mopW_LineEdit_NewPlaylist->setText("");
}


// **************************************************************************
void MainWindow::on_mopW_PushButton_DelPlaylist_clicked()
{
	int aiIndex = ui->mopW_TableWidget_Playlists->currentRow();
	moManageSetLists.mvDelSetlist(aiIndex) ;
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
	moManageSetLists.mvDelTrack(iIndex);
}


// **************************************************************************
void MainWindow::on_mopW_TableWidget_Tracks_currentCellChanged(
		int currentRow,
		int /*currentColumn*/,
		int previousRow,
		int /*previousColumn*/)
{
	ui->mopW_PushButton_DelTrack->setEnabled(currentRow != -1);
}
