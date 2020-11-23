#include "DialogGetPassword_d.h"
#include "ui_DialogGetPassword_d.h"

#include <QPushButton>

DialogGetPassword_d::DialogGetPassword_d(quint64& aurKey, QString* aspReminder, QWidget *parent) :
	QDialog(parent)
	,ui(new Ui::DialogGetPassword_d)
	,murKey(aurKey)
	,mspReminder(aspReminder)
{
	ui->setupUi(this);

	ui->mopW_LineEdit_Password->setInputMask( QString("9").repeated(16) ); // Just numbers!
	ui->mopW_ButtonBox->button(QDialogButtonBox::Ok)->setEnabled( false );

	if(nullptr ==aspReminder) {
		ui->mop_W_LabelReminder->hide();
		ui->mopW_LineEdit_Reminder->hide();
	}
}


// ****************************************************************************
DialogGetPassword_d::~DialogGetPassword_d()
{
	delete ui;
}


// ****************************************************************************
void
DialogGetPassword_d::on_mopW_LineEdit_Password_textChanged()
{
	bool bInvalidPwd  = ui->mopW_LineEdit_Password->text().count() < 4;
	ui->mopW_ButtonBox->button(QDialogButtonBox::Ok)->setEnabled( !bInvalidPwd );
}


// ****************************************************************************
void
DialogGetPassword_d::on_mopW_ButtonBox_accepted()
{
	murKey = ui->mopW_LineEdit_Password->text().toULongLong();
	if(nullptr != mspReminder) {
		*mspReminder = ui->mopW_LineEdit_Reminder->text();
	}
	ui->mopW_LineEdit_Password->setText("12345678");
}


// ****************************************************************************
void
DialogGetPassword_d::on_mopW_ButtonBox_rejected()
{
	ui->mopW_LineEdit_Password->setText("12345678");
}
