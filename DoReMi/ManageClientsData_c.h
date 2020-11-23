#ifndef MANAGE_CLIENT_DATA_C_H
#define MANAGE_CLIENT_DATA_C_H

// Prj
#include "TestProbe/TestProbe_b.h"

// Qt
#include <QTableWidget>
#include <QString>


// Classe auxiliar para manusear dados de users entre tela/memória/arquivo
// Podera ser destacada e seguir "vida propria"
class ManageClientsData_c: public TestProbe_b {
public:

	const static char msCryptoTest[] ;

	struct ClientData_s {
		QString     sName ;
		QString     sDateTime ;      // da Criacao do registro
		QByteArray  sCryptoTest ;    // precisa dar sempre o mesmo número.
		QByteArray  sClient_ID ;
		QByteArray  sClient_Secret ;
		QString     sReminder ;
	};

	ManageClientsData_c();

	void mvFromFile() ;
	void mvToFile() ;

	void mvAddElement(ClientData_s& aorUserData) ;
	void mvDelElement(int aiIndex) ;

	bool mbSetOrder(bool abCrescent) ;
	void mvSetTableWidget(QTableWidget* aopW_Table);

	ClientData_s moGetDataItem(uint16_t auItem);

private:
	void mvDataToTable() ;
	constexpr static char msFileName[] = "Tokens.json" ;

	QTableWidget* mopW_TableWidget = nullptr;
	std::vector<ClientData_s> movClientData ;

private: //TestProbe_b Interface
	std::string vsMyName() {return "";}
	uint32_t vsTestOrdinary()    {return 0;}
	uint32_t vsTestRegressions() {return 0;}
	uint32_t vsTestMembers()     {return 0;}
	uint32_t vsTestInherited()   {return 0;}
} ;

#endif // MANAGE_CLIENT_DATA_C_H
