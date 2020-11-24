#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Prj
#include "DialogClient_d.h"
#include "ManagePlaylists_c.h"
#include "MediaPlayer_c.h"
#include "SpotifyAPI/SpotifyAPI_c.h"
#include "TestProbe/TestProbe_b.h"


// Qt


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow, public TestProbe_b
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

	void on_mopW_PushButton_StopAudio_clicked();

private:
	Ui::MainWindow* ui;

	std::vector<SpotifyAPI_c::SearchTrackItems_s> movSearchResult;

	ManagePlaylists_c moManagePlaylists ;
	SpotifyAPI_c      moSpotifyAPI ; // Manager de comunicacao com a API do Spotify

	MediaPlayer_c moAudioPlayer;

	void mvBtnAdd_Test();

	void mvSignalsTableWidget_Search() ;
	void mvSignalsTableWidget_Playlists() ;
	void mvSignalsTableWidget_Tracks() ;


private: //TestProbe_b Interface
	std::string vsMyName() {return "";}
	uint32_t vsTestOrdinary()    {return 0;}
	uint32_t vsTestRegressions() {return 0;}
	uint32_t vsTestMembers()     {return 0;}
	uint32_t vsTestInherited()   {return 0;}

};
#endif // MAINWINDOW_H
