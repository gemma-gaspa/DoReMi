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




	//moManageSetLists
	moManageSetLists.mvSetWidgets(ui->mopW_TableWidget_Playlists, ui->mopW_ComboBox_Users);
	moManageSetLists.mvFromFile() ;
}


// ****************************************************************************
MainWindow::~MainWindow()
{
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


	std::vector<SpotifyAPI_c::SearchTrackItems_s> ovResult;
	QString sSentence = ui->mopW_LineEdit_Search->text();

	uint32_t uNumTracks = ui->mopW_LineEdit_MaxTracks->text().toUInt();

	bool bSetFlag = ui->mopW_CheckBox_30s->isChecked();
	uint32_t uSearchFlags = bSetFlag? uint32_t(SpotifyAPI_c::Flags_e::eIS_PLAYABLE_30s) : 0;

	uint32_t uAccessTime_ms = moSpotifyAPI.movSearchTrack(
				sSentence,
				uNumTracks,
				uSearchFlags,
				ovResult);

	ui->mopW_TextBrowser_Out->setText("Tempo total de acesso: "+QString::number(uAccessTime_ms/1000.0));


	// Populate Data
	for(uint16_t u=0 ; u<ovResult.size() ; u++) {
		ui->mopW_TableWidget_Search->insertRow(u);

		// Nao ha vazamento de memoria: QTableWidget apagara os QTableWidgetItem
		QTableWidgetItem* opName   = new QTableWidgetItem(ovResult[u].sName);
		QTableWidgetItem* opArtist = new QTableWidgetItem(ovResult[u].ovArtists[0].sName);
		QTableWidgetItem* opAlbum  = new QTableWidgetItem(ovResult[u].oAlbum.sName);
		QTableWidgetItem* opDate   = new QTableWidgetItem(ovResult[u].oAlbum.sReleaseDate);

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
void MainWindow::on_mopW_ComboBox_Users_currentIndexChanged(int aiIndex)
{
	moManageSetLists.mvSetUser(aiIndex);
}
