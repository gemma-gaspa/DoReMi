#ifndef MANAGEUSERDATA_C_H
#define MANAGEUSERDATA_C_H


#include <QTableWidget>
#include <QString>


// Classe auxiliar para manusear dados de users entre tela/memória/arquivo
// Podera ser destacada e seguir "vida propria"
class ManageUserData_c {
public:

	const static char msCryptoTest[] ;

	struct UserData_s {
		QString     sName ;
		QString     sDateTime ;      // da Criacao do registro
		QByteArray  sCryptoTest ;    // precisa dar sempre o mesmo número.
		QByteArray  sClient_ID ;
		QByteArray  sClient_Secret ;
		QString     sReminder ;
	};

	ManageUserData_c();

	void mvFromFile() ;
	void mvToFile() ;

	void mvAddElement(UserData_s& aorUserData) ;
	void mvDelElement(int aiIndex) ;

	bool mbSetOrder(bool abCrescent) ;
	void mvSetTableWidget(QTableWidget* aopW_Table);

	UserData_s moGetDataItem(uint16_t auItem);

private:
	void mvDataToTable() ;
	constexpr static char msFileName[] = "Tokens.json" ;

	QTableWidget* mopW_TableWidget = nullptr;
	std::vector<UserData_s> movUserData ;

	void DrawTable() ;

} ;

#endif // MANAGEUSERDATA_C_H
