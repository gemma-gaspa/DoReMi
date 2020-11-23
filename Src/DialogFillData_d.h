#ifndef DIALOG_FILL_DATA_D_H
#define DIALOG_FILL_DATA_D_H

// rad
#include <QDialog>

// Prj
#include "ManageClientsData_c.h"
#include "TestProbe/TestProbe_b.h"



namespace Ui {
	class DialogFillData_d;
}

class DialogFillData_d : public QDialog, public TestProbe_b
{
	Q_OBJECT

public:
	explicit DialogFillData_d(ManageClientsData_c::ClientData_s& aorData, QWidget *parent = nullptr);
	~DialogFillData_d();

private slots:
	void on_mopW_ButtonBox_rejected();

	void on_mopW_ButtonBox_accepted();

	void on_mopW_LineEdit_Name_textChanged(const QString&);

	void on_mopW_LineEdit_ClientID_textChanged(const QString&);

	void on_mopW_LineEdit_ClientSecret_textChanged(const QString&);

	void on_mopW_PushButton_VerSenha_clicked();

private:
	Ui::DialogFillData_d *ui;

	const QString msTimeFormat = "yyyy/MM/dd  -  HH:mm:ss";

	ManageClientsData_c::ClientData_s& morData;

	void mvEnableOkBtnTest();

private: //TestProbe_b Interface
	std::string vsMyName() {return "";}
	uint32_t vsTestOrdinary()    {return 0;}
	uint32_t vsTestRegressions() {return 0;}
	uint32_t vsTestMembers()     {return 0;}
	uint32_t vsTestInherited()   {return 0;}
};

#endif // DIALOG_FILL_DATA_D_H
