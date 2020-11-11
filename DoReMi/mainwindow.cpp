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

//	QUrl sEndpoint("https://api.spotify.com/v1/search");
	QUrl sEndpoint("https://www.google.com/search?q=video");


	QNetworkAccessManager *opWebManager = new QNetworkAccessManager(this);

	connect(opWebManager, &QNetworkAccessManager::finished, this, &MainWindow::replyFinished); // signal to slot

	opWebManager->get( QNetworkRequest(sEndpoint) );

//	delete opWebManager;
}


// ****************************************************************************
void
MainWindow::replyFinished(QNetworkReply* aopReply)
{
	ui->opW_TextBrowserOut->setText("MainWindow::replyFinished: " + QString::number(aopReply->isRunning()));

	aopReply->deleteLater();
}


