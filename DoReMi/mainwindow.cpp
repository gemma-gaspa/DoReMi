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

	mspReturnPost = new QByteArray ;
}


// ****************************************************************************
MainWindow::~MainWindow()
{
	delete mspReturnPost  ;
	delete ui;
}


// ****************************************************************************
void
MainWindow::on_mopW_PushButton_TestaSpotify_clicked()
{
	QByteArray  sClient_ID     = "c7964ee8177b6" ;
	QByteArray  sClient_Secret = "2f71244f" ;
	QByteArray  sKey =  (sClient_ID + ":" + sClient_Secret).toBase64(); // Codifica para Base64


	ui->mopW_TextBrowser_Out->setText("Requisitando autorização de cliente...");

//	QUrl sEndpoint("https://api.spotify.com/authorize");
//	QUrl sEndpoint("https://api.spotify.com/api/token");
	QUrl sEndpoint("https://accounts.spotify.com/api/token?grant_type=client_credentials");

	QNetworkAccessManager *opWebManager = new QNetworkAccessManager(this);

	//      source         signal                           dest          function to execute
	connect(opWebManager, &QNetworkAccessManager::finished, this        , &MainWindow::replyFinished);
	connect(opWebManager, &QNetworkAccessManager::finished, opWebManager, &QNetworkAccessManager::deleteLater);

	QNetworkRequest oWebRequest(sEndpoint) ;
	oWebRequest.setRawHeader("Authorization", "Basic " + sKey);


	opWebManager->post( oWebRequest, *mspReturnPost );

//	delete opWebManager; memory leak!
}


// ****************************************************************************
void
MainWindow::replyFinished(QNetworkReply* aopReply)
{
	QByteArray sContent = aopReply->readAll();

	QString sToText = "MainWindow::replyFinished: " + QString(aopReply->isFinished() ? "Terminou" : "Esperandoooo" ) ;

	QString sError = aopReply->errorString();

	// Relatoriozinho:
	if("" == sError) {
		sToText += "\n\n" + sContent ;

	} else {
		sToText += "\n  " + sError ;
		if(sError == "TLS initialization failed") {
			sToText += "\n       https://stackoverflow.com/questions/53805704/tls-initialization-failed-on-get-request/53809217" ;
		}

		sToText += "\n\n" + sContent ;
	}

	ui->mopW_TextBrowser_Out->setText( sToText );


//	aopReply->deleteLater();
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

	for(size_t u=0 ; u<ovResult.size() ; u++) {
		// Fill the table
	}
}
