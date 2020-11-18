#include "DialogFillData_d.h"
#include "ui_DialogFillData_d.h"


// ****************************************************************************
DialogFillData_d::DialogFillData_d(ManageUserData_c::UserData_s& aorData, QWidget *parent) :
	QDialog(parent)
	,ui(new Ui::DialogFillData_d)
	,morData(aorData)
{
	ui->setupUi(this);
	// Config Janela
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Desliga botao '?'
	setFixedSize(size()); // Nao permite redimensionamento

	mvEnableOkBtnTest();
	ui->mopW_LineEdit_Password->setInputMask( QString("9").repeated(10) ); // Just numbers!
}


// ****************************************************************************
DialogFillData_d::~DialogFillData_d()
{
	delete ui;
}


// ****************************************************************************
void
DialogFillData_d::on_mopW_LineEdit_Name_textChanged(const QString&)
{
	mvEnableOkBtnTest();
}


// ****************************************************************************
void
DialogFillData_d::on_mopW_LineEdit_ClientID_textChanged(const QString&)
{
	mvEnableOkBtnTest();
}


// ****************************************************************************
void
DialogFillData_d::on_mopW_LineEdit_ClientSecret_textChanged(const QString&)
{
	mvEnableOkBtnTest();
}


// ****************************************************************************
void
DialogFillData_d::on_mopW_LineEdit_Password_textChanged(const QString&)
{
	mvEnableOkBtnTest();
}


// ****************************************************************************
// obtem dados de todos botoes. Se um deles nao estiver de acordo, sinaliza
void
DialogFillData_d::mvEnableOkBtnTest()
{
	QString sNewLabelError = "Forneça ";

	bool bEnableOkBtn = false ;
	bool bInvalidName = ui->mopW_LineEdit_Name->text().count() < 5;
	bool bInvalidId   = ui->mopW_LineEdit_ClientID->text().count()     < 32;
	bool bInvalidSecr = ui->mopW_LineEdit_ClientSecret->text().count() < 32;
	bool bInvalidPwd  = ui->mopW_LineEdit_Password->text().count() < 5;

	if(bInvalidName) {
		sNewLabelError = "Nome: " + sNewLabelError + QString::number(5) ;

	} else if(bInvalidId) {
		sNewLabelError = "Client ID: "     + QString::number(32) ;

	} else if(bInvalidSecr) {
		sNewLabelError = "Client Secret: " + QString::number(32) ;

	} else if(bInvalidPwd) {
		sNewLabelError = "Password :" + QString::number(5) ;

	} else {
		sNewLabelError = "Parece tudo OK!";
		bEnableOkBtn = true ;
	}

	if(!bEnableOkBtn) {
		sNewLabelError += " ou mais caracteres.";
	}

	ui->mopW_LabelError->setText(sNewLabelError);

	ui->mopW_ButtonBox->button(QDialogButtonBox::Ok)->setEnabled( bEnableOkBtn );
}


// ****************************************************************************
#include "SimpleCryptQt.h"
#include <QTime>
void
DialogFillData_d::on_mopW_ButtonBox_accepted()
{
	// Criptografar e salvar na struct

	morData.sName     = ui->mopW_LineEdit_Name->text();
	morData.sReminder = ui->mopW_LineEdit_Reminder->text();
	morData.sDateTime = QDateTime::currentDateTime().toString(msTimeFormat) ;

	SimpleCryptQt oCrypto;
	quint64 uKey = ui->mopW_LineEdit_Password->text().toULongLong();

	oCrypto.mSetKey(uKey) ;

	morData.sClient_ID     = oCrypto.mEncryptToByteArray(ui->mopW_LineEdit_ClientID->text(    ).toUtf8());
	morData.sClient_Secret = oCrypto.mEncryptToByteArray(ui->mopW_LineEdit_ClientSecret->text().toUtf8());
	morData.sCryptoTest    = oCrypto.mEncryptToByteArray(QByteArray(ManageUserData_c::msCryptoTest));

	// Sem rastros na memoria da chave original:
	if(1) {
		uKey = 12345678;
		oCrypto.mSetKey(12345678) ;
		ui->mopW_LineEdit_Password->setText("12345678");
	}
}


// ****************************************************************************
void
DialogFillData_d::on_mopW_ButtonBox_rejected()
{
	// Sinaliza que nada mudou, deixando tudo em branco
	morData.sName = "";
	// GoodBye
}


// ****************************************************************************
void
DialogFillData_d::on_mopW_PushButton_VerSenha_clicked()
{
	QString x = QString::number( qrand() );
	ui->mopW_LineEdit_Name->setText("Nome_"+x);
	ui->mopW_LineEdit_Reminder->setText("Reminder_"+x);
	ui->mopW_LineEdit_Password->setText("123456");
	ui->mopW_LineEdit_ClientID->setText("ClientID_0000000000000000000000"+x);
	ui->mopW_LineEdit_ClientSecret->setText("ClientSecret_00000000000000000"+x);
}
