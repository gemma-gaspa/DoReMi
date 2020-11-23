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
	mvPlayNext(moMediaPlayer.state()) ;
}


// **************************************************************************
void
MediaPlayer_c::mvPlayNext(QMediaPlayer::State aeState)
{
	switch(aeState) {
		case QMediaPlayer::State::PausedState :
		{
			if(!mbKeepPaused) {
				moMediaPlayer.play();
			}
			mbKeepPaused = false ;
		}
		break;

		// Next track logic:
		case QMediaPlayer::State::StoppedState :
		{
			if(muPlayPosition<movPlaylist.size()) {

				// Play Next track
				QUrl sLink = movPlaylist[muPlayPosition].sPlayLink ;
				moMediaPlayer.setMedia(sLink);

				muPlayPosition++;
				moMediaPlayer.play();
			} else {
				muPlayPosition=0;
			}
		}
		break;

		default: ;
	}

	//QString             sError = moMediaPlayer.errorString();
	//QMediaPlayer::Error eError = moMediaPlayer.error();
}


// **************************************************************************
void
MediaPlayer_c::mvPause()
{
	if(QMediaPlayer::State::PlayingState == moMediaPlayer.state()) {
		mbKeepPaused = true ;
		moMediaPlayer.pause();
	}
}


// **************************************************************************
void
MediaPlayer_c::mvStop()
{
	bool bForceStop = QMediaPlayer::State::StoppedState != moMediaPlayer.state() ;

	muPlayPosition = bForceStop ? uint32_t(-1) : 0;
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

