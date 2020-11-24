#ifndef MANAGE_PALYLISTS_C_H
#define MANAGE_PALYLISTS_C_H

// Prj
#include "TestProbe/TestProbe_b.h"

// C/C++
#include <vector>

// Qt
#include <QComboBox>
#include <QTableWidget>
#include <QString>


// Classe auxiliar para manusear dados de users entre tela/memória/arquivo
// Podera ser destacada e seguir "vida propria"
class ManagePlaylists_c : public TestProbe_b {
public:

	const static char msCryptoTest[] ;


	struct UsersData_s {
		QString sName;
		struct Playlists_s {
			struct Track_s {
				QString sTrackName;
				QString sAlbumName;
				QString sArtist;
				QString sImage;
				QString sPreviewURL;
				QString sReleaseDate;
			};


			QString sName;
			std::vector<Track_s> ovTracks ;
		};

		std::vector<Playlists_s> ovPlaylists;
	};

	ManagePlaylists_c();

	void mvFromFile() ;
	void mvToFile() ;

	void mvSetActiveUser(int aiIndex);
	void mvSetActivePlaylist(int aiIndex);

	void mvAddUser(const QString& aorName);
	void mvDelUser(int aiIndex) ;

	void mvAddPlaylist(const QString& aorName);
	void mvDelPlaylist(int aiIndex) ;

	void mvAddTrack(UsersData_s::Playlists_s::Track_s& aorTrack);
	void mvDelTrack(int aiIndex) ;

	bool mbSetOrder(bool abCrescent) ;
	void mvSetWidgets(
			QTableWidget* aopW_TablePlaylists,
			QTableWidget* aopW_TableTracks,
			QComboBox*    aopW_ComboBoxUsers);

	UsersData_s::Playlists_s::Track_s moGetDataTrack(int aiTrackIndex);

private:
	void mvDataToPlaylistTable() ;
	void mvDataToTracksTable() ;
	void mvDataToComboBox();
	constexpr static char msFileName[] = "../UsersData.json" ;

	QTableWidget* mopW_TablePlaylists = nullptr;
	QTableWidget* mopW_TableTracks   = nullptr;
	QComboBox*    mopW_ComboBoxUsers = nullptr;

	std::vector<UsersData_s> movUsersData ;

	int miCurrentUserIndex = -1;
	int miCurrentPlaylistIndex = -1;


private: //TestProbe_b Interface
	std::string vsMyName() {return "";}
	uint32_t vsTestOrdinary()    {return 0;}
	uint32_t vsTestRegressions() {return 0;}
	uint32_t vsTestMembers()     {return 0;}
	uint32_t vsTestInherited()   {return 0;}

} ;

#endif // MANAGE_PALYLISTS_C_H
