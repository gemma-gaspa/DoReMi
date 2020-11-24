
// Prj
#include "ManageClientsData_c.h"

// Qt
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>


const char ManageClientsData_c::msCryptoTest[] ="123";


// **************************************************************************
ManageClientsData_c::ManageClientsData_c()
{

}


// **************************************************************************
void
ManageClientsData_c::mvFromFile()
{
	QFile sDataFile(msFileName);
	sDataFile.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Unbuffered);
	QByteArray sRawDataFile = sDataFile.readAll();
	sDataFile.close();

	QJsonParseError oErrorParser;
	QJsonDocument   oJsonDoc = QJsonDocument::fromJson(sRawDataFile, &oErrorParser);

	movClientData.clear();
	if (!oJsonDoc.isNull()) {
		QJsonArray oArray = oJsonDoc.array() ;
		foreach(const QJsonValue& oJsonElem, oArray) {
			ClientData_s oElem;
			oElem.sName          = oJsonElem["sName"         ].toString();
			oElem.sDateTime      = oJsonElem["sDateTime"     ].toString();
			oElem.sClient_ID     = oJsonElem["sClient_ID"    ].toString().toUtf8();
			oElem.sClient_Secret = oJsonElem["sClient_Secret"].toString().toUtf8();
			oElem.sCryptoTest    = oJsonElem["sCryptoTest"   ].toString().toUtf8();
			oElem.sReminder      = oJsonElem["sLembrete"     ].toString();

			// Dados criptografados:
			oElem.sClient_ID     = QByteArray::fromBase64(oElem.sClient_ID);
			oElem.sClient_Secret = QByteArray::fromBase64(oElem.sClient_Secret);
			oElem.sCryptoTest    = QByteArray::fromBase64(oElem.sCryptoTest);

			movClientData.push_back(oElem) ;
		}
	}

	// pra tela
	mvDataToTable() ;
}


// **************************************************************************
void
ManageClientsData_c::mvToFile()
{
	QJsonArray oArray ;
	for(uint16_t u=0 ; u<movClientData.size() ; u++) { // sim, eu sei que dá pra usar iterator.
		ClientData_s oElem = movClientData[u];

		oElem.sClient_ID     = oElem.sClient_ID.toBase64();
		oElem.sClient_Secret = oElem.sClient_Secret.toBase64();
		oElem.sCryptoTest    = oElem.sCryptoTest.toBase64();

		// https://forum.qt.io/topic/75800/how-to-add-qjsonarray-to-qjsonobject
		// Cria elemento da array
		QJsonObject oJsonObj  ;
		oJsonObj["sName"         ] =         oElem.sName          ;
		oJsonObj["sClient_ID"    ] = QString(oElem.sClient_ID    );
		oJsonObj["sClient_Secret"] = QString(oElem.sClient_Secret);
		oJsonObj["sCryptoTest"   ] = QString(oElem.sCryptoTest   );
		oJsonObj["sLembrete"     ] =         oElem.sReminder      ;
		oJsonObj["sDateTime"     ] =         oElem.sDateTime      ;

		oArray.append(oJsonObj);
	}

	QByteArray sRawDataFile = QJsonDocument(oArray).toJson() ;

	QFile sDataFile( msFileName ) ;
	sDataFile.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Unbuffered ) ;
	sDataFile.write( sRawDataFile ) ;
	sDataFile.close() ;
}


// **************************************************************************
void
ManageClientsData_c::mvAddElement(ClientData_s& aorUserData)
{
	movClientData.push_back(aorUserData) ;
	mvDataToTable() ;
}


// **************************************************************************
void
ManageClientsData_c::mvDelElement(int aiIndex)
{
	if(aiIndex >=0  && aiIndex < int(movClientData.size())) {
		auto it = movClientData.begin();
		movClientData.erase(it);
	}
	mvDataToTable() ;
}


// **************************************************************************
bool
ManageClientsData_c::mbSetOrder(bool abCrescent)
{

}


// **************************************************************************
void
ManageClientsData_c::mvSetTableWidget(QTableWidget *aopW_Table)
{
	mopW_TableWidget = aopW_Table ;
}


// **************************************************************************
void
ManageClientsData_c::mvDataToTable()
{
	if(nullptr != mopW_TableWidget) {
		mopW_TableWidget->setRowCount(0); // Erase All

		// Reorder


		for(uint16_t u=0 ; u<movClientData.size() ; u++) {
			mopW_TableWidget->insertRow(u);
			// Nao ha vazamento de memoria: QTableWidget apagara os QTableWidgetItem
			QTableWidgetItem* opName = new QTableWidgetItem(movClientData[u].sName);
			QTableWidgetItem* opDate = new QTableWidgetItem(movClientData[u].sDateTime);

			// Itens nao-editaveis:
			opName->setFlags( opName->flags() &~Qt::ItemIsEditable );
			opDate->setFlags( opDate->flags() &~Qt::ItemIsEditable );
			mopW_TableWidget->setItem(u, 0, opName);
			mopW_TableWidget->setItem(u, 1, opDate);
		}
	}

	mopW_TableWidget->repaint();
}


// **************************************************************************
ManageClientsData_c::ClientData_s
ManageClientsData_c::moGetDataItem(uint16_t auItem)
{
	ClientData_s oRet;
	if(auItem < movClientData.size()) {
		oRet = movClientData[auItem];
	}
	return oRet;
}


