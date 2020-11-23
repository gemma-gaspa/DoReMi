#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Prj
#include "DialogClient_d.h"
#include "ManageSetlists_c.h"
#include "SpotifyAPI/SpotifyAPI_c.h"

// Qt
#include "MediaPlayer_c.h"


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
	void on_mopW_PushButton_ConfigClienteSpotify_clicked();

	void on_mopW_PushButton_Search_clicked();

	void on_mopW_ComboBox_Users_currentIndexChanged(int index);

	void on_mopW_PushButton_AdicionarTrack_clicked();

	void on_mopW_PushButton_AddUser_clicked();

	void on_mopW_PushButton_DelUser_clicked();

	void on_mopW_LineEdit_NewUser_textChanged(const QString& asrNewUser);

	void on_mopW_PushButton_AddPlaylist_clicked();

	void on_mopW_PushButton_DelPlaylist_clicked();

	void on_mopW_LineEdit_NewPlaylist_textChanged(const QString& asrNewPlaylist);

	void on_mopW_PushButton_DelTrack_clicked();


	void on_mopW_PushButton_PlayAudio_clicked();

	void on_mopW_PushButton_PauseAudio_clicked();

	void on_mopW_TableWidget_Search_cellChanged(int aiRow, int aiColumn);
	void on_mopW_TableWidget_Playlists_cellChanged(int aiRow, int aiColumn);
	void on_mopW_TableWidget_Tracks_cellChanged(int aiRow, int aiColumn);

	void on_mopW_TableWidget_Search_cellPressed(int, int );
	void on_mopW_TableWidget_Playlists_cellPressed(int, int );
	void on_mopW_TableWidget_Tracks_cellPressed(int , int );

private:
	Ui::MainWindow* ui;

	std::vector<SpotifyAPI_c::SearchTrackItems_s> movSearchResult;

	ManageSetlists_c moManageSetLists ;
	SpotifyAPI_c     moSpotifyAPI ; // Manager de comunicacao com a API do Spotify

	MediaPlayer_c moAudioPlayer;

	void mvBtnAdd_TestEnable();

	void mvSignalsTableWidget_Search() ;
	void mvSignalsTableWidget_Playlists() ;
	void mvSignalsTableWidget_Tracks() ;

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

