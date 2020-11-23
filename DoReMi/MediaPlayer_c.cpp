#include "MediaPlayer_c.h"

#include <QMediaPlayer>

// **************************************************************************
MediaPlayer_c::MediaPlayer_c(QObject *parent) :
	QObject(parent),
	moMediaPlayer(this)
{
	mvSetVolume(0.251f);

	//      source           signal                      dest obj  function to execute (slot)
	connect(&moMediaPlayer, &QMediaPlayer::stateChanged, this    , &MediaPlayer_c::mvPlayNext); // signal para renovar token, depois de menos que 1h

}


// **************************************************************************
void
MediaPlayer_c::mvSetPlaylist(std::vector<PlaylistItem_s>& aorPlaylist)
{
	movPlaylist = aorPlaylist;
}


// **************************************************************************
void
MediaPlayer_c::mvPlay()
{
	muPlayPosition=0;
	mvPlayNext() ;
}


// **************************************************************************
void
MediaPlayer_c::mvPlayNext(QMediaPlayer::State aeState)
{
	if(QMediaPlayer::State::StoppedState != aeState) {
		return ;
	}


	if(muPlayPosition<movPlaylist.size()) {
		QMediaPlayer::Error eError;
		QString	            sError;

		QString sLink = movPlaylist[muPlayPosition].sPlayLink ;
		moMediaPlayer.setMedia(QUrl(sLink));
		moMediaPlayer.play();

		eError = moMediaPlayer.error();
		sError = moMediaPlayer.errorString();
		muPlayPosition++;
	}
}


// **************************************************************************
void
MediaPlayer_c::mvPause()
{
	moMediaPlayer.pause();
}


// **************************************************************************
void
MediaPlayer_c::mvStop()
{
	moMediaPlayer.stop();
}


// **************************************************************************
// de 0 a 1
void
MediaPlayer_c::mvSetVolume(float afVol)
{
	afVol = afVol*100+0.5f;
	if(afVol<0) {
		afVol = 0;
	}

	if(afVol>100) {
		afVol = 100;
	}
	moMediaPlayer.setVolume(int(afVol));
}

/*


	moAudioPlayer.play();

	eError = moAudioPlayer.error();
	sError = moAudioPlayer.errorString();
*/

