#include "mainwindow.h"
#include "ui_mainwindow.h"


// Qt
#include <QNetworkAccessManager>
#include <QNetworkReply>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}


// ****************************************************************************
void
MainWindow::on_opW_PushButton_TestaSpotify_clicked()
{
	ui->opW_TextBrowserOut->setText("Requisitando procura...");

	QUrl sEndpoint("https://api.spotify.com/v1/search?q=1234");
//	QUrl sEndpoint("https://www.google.com/search?q=video");

	// https://www.bogotobogo.com/Qt/Qt5_Downloading_Files_QNetworkAccessManager_QNetworkRequest.php
	QNetworkAccessManager *opWebManager = new QNetworkAccessManager(this);


	connect(opWebManager, &QNetworkAccessManager::finished, this, &MainWindow::replyFinished); // signal to slot

	connect(opWebManager, &QNetworkAccessManager::finished, opWebManager, &QNetworkAccessManager::deleteLater);

	QNetworkRequest oWebRequest(sEndpoint) ;

	QByteArray SPOTIFY_CLIENT_ID = "1234";
	QByteArray SPOTIFY_CLIENT_SECRET = "5678";

	QByteArray key =  (SPOTIFY_CLIENT_ID + ":" + SPOTIFY_CLIENT_SECRET).toBase64();

	oWebRequest.setRawHeader("Authorization", "Bearer " +key);
	opWebManager->get( oWebRequest );

//	delete opWebManager;
}


// ****************************************************************************
void
MainWindow::replyFinished(QNetworkReply* aopReply)
{
	QByteArray sContent = aopReply->readAll();

	QString sToText = "MainWindow::replyFinished: " + QString(aopReply->isFinished() ? "Terminou" : "Esperandoooo" ) ;

	QString sError = aopReply->errorString();

	if("" == sError) {
		sToText += "\n\n" +sContent ;

	} else {
		sToText += "\n  " + sError ;
		if(sError == "TLS initialization failed") {
			sToText += "\n       https://stackoverflow.com/questions/53805704/tls-initialization-failed-on-get-request/53809217" ;
		}

		sToText += "\n\n" + sContent ;
	}

	ui->opW_TextBrowserOut->setText( sToText );



//	aopReply->deleteLater();
}


