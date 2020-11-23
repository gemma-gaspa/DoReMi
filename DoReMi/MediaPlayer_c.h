#ifndef MEDIA_PLAYER_C_H
#define MEDIA_PLAYER_C_H

#include <QObject>
#include <QMediaPlayer>

class MediaPlayer_c : public QObject
{
	Q_OBJECT
public:

	struct PlaylistItem_s {
		QString sName ;
		QString sPlayLink;
		QString sAlbum;
	};


	explicit MediaPlayer_c(QObject *parent = nullptr);

	void mvSetPlaylist(std::vector<PlaylistItem_s>& aorPlaylist);

	void mvPlay();
	void mvPause() ;
	void mvStop();

	void mvSetVolume(float afVol); // de 0 a 1

signals:



private:
	void mvPlayNext(QMediaPlayer::State aeState);

	std::vector<PlaylistItem_s> movPlaylist;

	QMediaPlayer moMediaPlayer;
	uint32_t muPlayPosition = 0;

	bool mbKeepPaused  = false ;
};

#endif // MEDIA_PLAYER_C_H
