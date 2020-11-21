#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


// Projeto
#include "DialogClient_d.h"
#include "SpotifyAPI_c.h"

// Qt
#include <QNetworkAccessManager>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_mopW_PushButton_TestaSpotify_clicked();
	void on_mopW_PushButton_ConfigClienteSpotify_clicked();

	void on_mopW_PushButton_Search_clicked();

private:
	Ui::MainWindow *ui;

	QByteArray* mspReturnPost = nullptr ; // verificar importancia desta coisa.

	SpotifyAPI_c moSpotifyAPI ; // Manager de comunicacao com a API do Spotify

	void replyFinished(QNetworkReply* aopReply);


};
#endif // MAINWINDOW_H


/*

  https://www.qtcentre.org/threads/57920-Where-to-use-QTableView-and-QTableWidget

  https://stackoverflow.com/questions/25386438/creating-a-simple-table-cell-in-qt

ui->mopW_TableWidget->setHorizontalHeaderLabels(QStringList() << "Nome" << "Data Criação");



	// Configura tabela
	ui->mopW_TableView->setModel(&moModel);
	moModel.insertColumns(0, 2);

	moModel.setHeaderData(0, Qt::Horizontal, "Nome");
	moModel.setHeaderData(1, Qt::Horizontal, "Data Criação");


	ui->mopW_TableView->setHorizontalHeader(&moHorizHeader);

	moHorizHeader.setSectionsMovable(false);


	ui->mopW_TableView->setColumnWidth(0, 220);
	ui->mopW_TableView->setColumnWidth(1, 220);








	if(nullptr != mopW_TableView) {
		int iNumRows = mopW_TableView->model()->rowCount() ;
		mopW_TableView->model()->removeRows(0, iNumRows);

		// Reorder


		QModelIndex index = mopW_TableView->currentIndex();
		for(uint16_t u=0 ; u<mvUserData.size() ; u++) {
			mopW_TableView->model()->insertRow(u);
			QModelIndex oModelIdx(u);

			mopW_TableView->model()->setItemData(index);
					index++;
		}
	}
  */

