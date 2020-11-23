#ifndef MANAGE_SETLISTS_C_H
#define MANAGE_SETLISTS_C_H

// C/C++
#include <vector>

// Qt
#include <QComboBox>
#include <QTableWidget>
#include <QString>


// Classe auxiliar para manusear dados de users entre tela/memória/arquivo
// Podera ser destacada e seguir "vida propria"
class ManageSetlists_c {
public:

	const static char msCryptoTest[] ;


	struct UsersData_s {
		QString sName;
		struct SetLists_s {
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

		std::vector<SetLists_s> ovSetLists;
	};

	ManageSetlists_c();

	void mvFromFile() ;
	void mvToFile() ;

	void mvSetActiveUser(int aiIndex);
	void mvSetActiveSetlist(int aiIndex);

	void mvAddUser(const QString& aorName);
	void mvDelUser(int aiIndex) ;

	void mvAddSetlist(const QString& aorName);
	void mvDelSetlist(int aiIndex) ;

	void mvAddTrack(UsersData_s::SetLists_s::Track_s& aorTrack);
	void mvDelTrack(int aiIndex) ;

	bool mbSetOrder(bool abCrescent) ;
	void mvSetWidgets(
			QTableWidget* aopW_TableSetlists,
			QTableWidget* aopW_TableTracks,
			QComboBox*    aopW_ComboBoxUsers);

	UsersData_s::SetLists_s::Track_s moGetDataTrack(int aiTrackIndex);

private:
	void mvDataToSetlistTable() ;
	void mvDataToTracksTable() ;
	void mvDataToComboBox();
	constexpr static char msFileName[] = "UsersData.json" ;

	QTableWidget* mopW_TableSetlists = nullptr;
	QTableWidget* mopW_TableTracks   = nullptr;
	QComboBox*    mopW_ComboBoxUsers = nullptr;

	std::vector<UsersData_s> movUsersData ;

	int miCurrentUserIndex = -1;
	int miCurrentSetlistIndex = -1;
} ;

#endif // MANAGE_SETLISTS_C_H
