#ifndef DIALOG_CLIENT_D_H
#define DIALOG_CLIENT_D_H

//Prj
#include "ManageUserData_c.h"

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

class DialogClient_d : public QDialog
{
	Q_OBJECT

public:
	explicit DialogClient_d(QWidget *parent = nullptr);
	~DialogClient_d();

private slots:
	void on_mopW_PushButton_Delete_clicked();

	void on_mopW_PushButton_Insert_clicked();

	void on_mopW_ButtonBox_rejected();

	void on_mopW_TableWidget_itemSelectionChanged();

	void on_mopW_ButtonBox_accepted();

private:
	Ui::DialogClient_d *ui;


private:

	// Classe auxiliar para manusear dados de users entre tela/memória/arquivo
	// Podera ser destacada e seguir "vida propria"
	ManageUserData_c moManageUserData;

};

#endif // DIALOG_CLIENT_D_H
