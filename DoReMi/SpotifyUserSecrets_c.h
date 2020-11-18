#ifndef SPOTIFY_USER_SECRETS_C_H
#define SPOTIFY_USER_SECRETS_C_H

#include <QByteArray>
#include <QDateTime>

class SpotifyUserSecrets_c {
public:
	QByteArray ID ;
	QByteArray Secret;
	QByteArray Token ;
	QDateTime  LastUpdate;
};

#endif // SPOTIFY_USER_SECRETS_C_H
