#ifndef DIALOGGETPASSWORD_D_H
#define DIALOGGETPASSWORD_D_H

#include <QDialog>

namespace Ui {
class DialogGetPassword_d;
}

class DialogGetPassword_d : public QDialog
{
	Q_OBJECT

public:
	explicit DialogGetPassword_d(quint64& aurKey, QString* aspReminder, QWidget *parent = nullptr);
	~DialogGetPassword_d();

private slots:
	void on_mopW_LineEdit_Password_textChanged();

	void on_mopW_ButtonBox_accepted();

	void on_mopW_ButtonBox_rejected();

private:
	Ui::DialogGetPassword_d *ui;

	// Retorno:
	quint64& murKey;
	QString* mspReminder = nullptr;
};

#endif // DIALOGGETPASSWORD_D_H
