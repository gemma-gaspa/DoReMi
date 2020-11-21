/*
 *
 *
 *
 *
 *
 * */


#include "DialogClient_d.h"
#include "ui_DialogClient_d.h"

// Prj
#include "DialogFillData_d.h"
#include "DialogGetPassword_d.h"
#include "SimpleCrypt/SimpleCryptQt.h"

// Qt
#include <QStandardItemModel>
#include <QTableWidget>
#include <QFile>


DialogClient_d::DialogClient_d(SpotifyUserSecrets_c& aorSpotifyUserSecrets, QWidget* parent) :
	QDialog(parent)
	,ui(new Ui::DialogClient_d)
	,morSpotifyUserSecrets(aorSpotifyUserSecrets)
{
	ui->setupUi(this);
	// Config Janela
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint); // Desliga botao '?'
	setFixedSize(size()); // Nao permite redimensionamento

	// Configura tabela
	ui->mopW_TableWidget->insertColumn(0);
	ui->mopW_TableWidget->insertColumn(1);
	//ui->mopW_TableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
	ui->mopW_TableWidget->resizeColumnToContents(1);

	// https://www.qtcentre.org/threads/21008-Change-QTableWidget-header-name-or-text
	// https://www.embeddeduse.com/2016/04/29/using-cpp-11-initializer-lists-in-qt-ways/
	QStringList svList = { "Nome", "Data Criação"} ;
	ui->mopW_TableWidget->setHorizontalHeaderLabels(svList);

	ui->mopW_TableWidget->setColumnWidth(0, 220);
	ui->mopW_TableWidget->setColumnWidth(1, 220);

	ui->mopW_PushButton_Delete->setEnabled(false);
	ui->mopW_PushButton_Adopt->setEnabled( false );


	moManageUserData.mvSetTableWidget(ui->mopW_TableWidget);

	// Ler arquivo
	moManageUserData.mvFromFile();
}


// **************************************************************************
DialogClient_d::~DialogClient_d()
{
	// Salvar arquivo
	delete ui;
}


// **************************************************************************
void
DialogClient_d::on_mopW_PushButton_Insert_clicked()
{
	ManageUserData_c::UserData_s oGetUserData ;

	DialogFillData_d oDialogFillData(oGetUserData, this);
	oDialogFillData.setModal(true);
	oDialogFillData.exec(); // espera sair

	// Insere dados
	if(oGetUserData.sName != "") {
		moManageUserData.mvAddElement(oGetUserData);
	}
}


// **************************************************************************
void
DialogClient_d::on_mopW_PushButton_Delete_clicked()
{
	// Se clicou eh porque ha algo selecionado na tabela.
	int iYpos = ui->mopW_TableWidget->currentRow() ;
	moManageUserData.mvDelElement(iYpos) ;
//	moManageUserData.mvToFile();
}


// **************************************************************************
void
DialogClient_d::on_mopW_PushButton_Cancel_clicked()
{
	mvExit();
}


// **************************************************************************
void
DialogClient_d::on_mopW_TableWidget_itemSelectionChanged()
{
	bool bSelected = ui->mopW_TableWidget->currentRow() >= 0 ;
	ui->mopW_PushButton_Delete->setEnabled(bSelected);
	ui->mopW_PushButton_Adopt->setEnabled( bSelected );
}


// **************************************************************************
void
DialogClient_d::on_mopW_PushButton_Save_clicked()
{
	moManageUserData.mvToFile();


	mvExit();
}


// **************************************************************************
void
DialogClient_d::on_mopW_PushButton_Adopt_clicked()
{
	int iRow = ui->mopW_TableWidget->currentRow() ;
	bool bSelected = iRow >= 0 ;
	if(bSelected) {
		ManageUserData_c::UserData_s oUserData = moManageUserData.moGetDataItem(uint16_t(iRow));

		quint64 uKey = 0;
		DialogGetPassword_d DialogGetPassword_d(uKey, nullptr, this);
		DialogGetPassword_d.setModal(true);
		DialogGetPassword_d.exec(); // espera sair

		// Fill unencrypted data
		SimpleCryptQt oCrypto;
		oCrypto.mSetKey(uKey) ;
		morSpotifyUserSecrets.ID     = oCrypto.msDecryptToByteArray(oUserData.sClient_ID);
		morSpotifyUserSecrets.Secret = oCrypto.msDecryptToByteArray(oUserData.sClient_Secret);
	}

	// Password
	mvExit();
}


// **************************************************************************
void
DialogClient_d::mvExit()
{
	this->done(QDialog::Accepted);
}




