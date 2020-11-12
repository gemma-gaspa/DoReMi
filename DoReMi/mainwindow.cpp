#include "mainwindow.h"
#include "ui_mainwindow.h"


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
MainWindow::on_opW_PushButton_TestaSpotify_clicked()
{
	QByteArray  sClient_ID     = "c7964ee8177b6" ;
	QByteArray  sClient_Secret = "2f71244f" ;
	QByteArray  sKey =  (sClient_ID + ":" + sClient_Secret).toBase64(); // Codifica para Base64


	ui->opW_TextBrowserOut->setText("Requisitando autorização de cliente...");

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

	ui->opW_TextBrowserOut->setText( sToText );


//	aopReply->deleteLater();
}


#include "SimpleCryptQt.h"
void MainWindow::on_opPushButtonCrypto_clicked()
{
	QByteArray sOriginalText = "oi cara de boi" ;

	SimpleCryptQt crypto;





	crypto.setKey(0x0c2ad4a4acb9f023) ; //same random number: key should match encryption key

	QByteArray sEncrypt = crypto.encryptToByteArray(sOriginalText);

	QByteArray plaintext = crypto.decryptToByteArray(sEncrypt);

	SimpleCryptQt::Error eError = crypto.lastError() ;

	qDebug() << int(eError) ;
}
