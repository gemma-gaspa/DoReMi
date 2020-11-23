#ifndef DIALOG_CLIENT_D_H
#define DIALOG_CLIENT_D_H

//Prj
#include "ManageClientsData_c.h"
#include "SpotifyAPI/SpotifyUserSecrets_c.h"
#include "TestProbe/TestProbe_b.h"

// moc
#include <QDialog>

// C/C++
#include <vector>

// Qt
#include <QDateTime>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QTableWidget>

namespace Ui {
class DialogClient_d;
}

class DialogClient_d : public QDialog, public TestProbe_b
{
	Q_OBJECT

public:
	explicit DialogClient_d(SpotifyUserSecrets_c& aorUserSecrets, QWidget *parent = nullptr);
	~DialogClient_d();

private slots:
	void on_mopW_PushButton_Delete_clicked();

	void on_mopW_PushButton_Insert_clicked();

	void on_mopW_TableWidget_itemSelectionChanged();

	void on_mopW_PushButton_Save_clicked();

	void on_mopW_PushButton_Adopt_clicked();

	void on_mopW_PushButton_Cancel_clicked();

private:
	Ui::DialogClient_d *ui;


private:

	// Classe auxiliar para manusear dados de users entre tela/memória/arquivo
	// Podera ser destacada e seguir "vida propria"
	ManageClientsData_c moManageClientsData;

	// Retorno da janela:
	SpotifyUserSecrets_c& morSpotifyUserSecrets;

	void mvExit() ;

private: //TestProbe_b Interface
	std::string vsMyName() {return "";}
	uint32_t vsTestOrdinary()    {return 0;}
	uint32_t vsTestRegressions() {return 0;}
	uint32_t vsTestMembers()     {return 0;}
	uint32_t vsTestInherited()   {return 0;}
};

#endif // DIALOG_CLIENT_D_H
