
// Prj
#include "ManageUserData_c.h"

// Qt
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>


const char ManageUserData_c::msCryptoTest[] ="123";


ManageUserData_c::ManageUserData_c()
{

}


// **************************************************************************
void
ManageUserData_c::mvFromFile()
{
	QFile sDataFile(msFileName);
	sDataFile.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Unbuffered);
	QByteArray sRawDataFile = sDataFile.readAll();
	sDataFile.close();

	QJsonParseError oErrorParser;
	QJsonDocument   oJsonDoc = QJsonDocument::fromJson(sRawDataFile, &oErrorParser);

	movUserData.clear();
	if (!oJsonDoc.isNull()) {
		QJsonArray oArray = oJsonDoc.array() ;
		foreach(const QJsonValue& oJsonElem, oArray) {
			UserData_s oElem;
			oElem.sName          = oJsonElem.toObject().value("sName"         ).toString();
			oElem.sDateTime      = oJsonElem.toObject().value("sDateTime"     ).toString();
			oElem.sClient_ID     = oJsonElem.toObject().value("sClient_ID"    ).toString().toUtf8();
			oElem.sClient_Secret = oJsonElem.toObject().value("sClient_Secret").toString().toUtf8();
			oElem.sCryptoTest    = oJsonElem.toObject().value("sCryptoTest"   ).toString().toUtf8();
			oElem.sReminder      = oJsonElem.toObject().value("sLembrete"     ).toString();

			// Dados criptografados:
			oElem.sClient_ID     = QByteArray::fromBase64(oElem.sClient_ID);
			oElem.sClient_Secret = QByteArray::fromBase64(oElem.sClient_Secret);
			oElem.sCryptoTest    = QByteArray::fromBase64(oElem.sCryptoTest);

			movUserData.push_back(oElem) ;
		}
	}

	// QDateTime::fromString( sTemp, msTimeFormat );
	// pra tela
	mvDataToTable() ;
}


// **************************************************************************
void
ManageUserData_c::mvToFile()
{
	QJsonArray oArray ;
	for(uint16_t u=0 ; u<movUserData.size() ; u++) { // sim, eu sei que dá pra usar iterator.
		UserData_s oElem = movUserData[u];

		oElem.sClient_ID     = oElem.sClient_ID.toBase64();
		oElem.sClient_Secret = oElem.sClient_Secret.toBase64();
		oElem.sCryptoTest    = oElem.sCryptoTest.toBase64();

		// https://forum.qt.io/topic/75800/how-to-add-qjsonarray-to-qjsonobject
		// Cria elemento da array
		QJsonObject data = QJsonObject {
			qMakePair(QString("sName"         ), QJsonValue(        oElem.sName         )),
			qMakePair(QString("sClient_ID"    ), QJsonValue(QString(oElem.sClient_ID    ))),
			qMakePair(QString("sClient_Secret"), QJsonValue(QString(oElem.sClient_Secret))),
			qMakePair(QString("sCryptoTest"   ), QJsonValue(QString(oElem.sCryptoTest   ))),
			qMakePair(QString("sLembrete"     ), QJsonValue(        oElem.sReminder     )),
			qMakePair(QString("sDateTime"     ), QJsonValue(        oElem.sDateTime     ))
		};

		oArray.append(data);
	}

	QByteArray sRawDataFile = QJsonDocument(oArray).toJson() ;

	QFile sDataFile( msFileName ) ;
	sDataFile.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Unbuffered ) ;
	sDataFile.write( sRawDataFile ) ;
	sDataFile.close() ;
}


// **************************************************************************
void
ManageUserData_c::mvAddElement(UserData_s& aorUserData)
{
	movUserData.push_back(aorUserData) ;
	mvDataToTable() ;
}


// **************************************************************************
void
ManageUserData_c::mvDelElement(int aiIndex)
{
	if(aiIndex >=0  && aiIndex < int(movUserData.size())) {
		auto it = movUserData.begin();
		movUserData.erase(it);
	}
	mvDataToTable() ;
}


// **************************************************************************
bool
ManageUserData_c::mbSetOrder(bool abCrescent)
{

}


// **************************************************************************
void
ManageUserData_c::mvSetTableWidget(QTableWidget *aopW_Table)
{
	mopW_TableWidget = aopW_Table ;
}


// **************************************************************************
void
ManageUserData_c::mvDataToTable()
{
	if(nullptr != mopW_TableWidget) {
		mopW_TableWidget->setRowCount(0); // Erase All

		// Reorder


		for(uint16_t u=0 ; u<movUserData.size() ; u++) {
			mopW_TableWidget->insertRow(u);
			// Nao ha vazamento de memoria: QTableWidget apagara os QTableWidgetItem
			QTableWidgetItem* opNome = new QTableWidgetItem(movUserData[u].sName);
			QTableWidgetItem* opData = new QTableWidgetItem(movUserData[u].sDateTime);

			// Itens nao-editaveis:
			opNome->setFlags( opNome->flags() &~Qt::ItemIsEditable );
			opData->setFlags( opData->flags() &~Qt::ItemIsEditable );
			mopW_TableWidget->setItem(u, 0, opNome);
			mopW_TableWidget->setItem(u, 1, opData);
		}
	}

	mopW_TableWidget->repaint();
}


