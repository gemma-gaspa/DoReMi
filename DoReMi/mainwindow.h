#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Qt
#include <QNetworkAccessManager>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_opW_PushButton_TestaSpotify_clicked();

	void on_opPushButtonCrypto_clicked();

private:
	Ui::MainWindow *ui;

	QByteArray* mspReturnPost = nullptr ;

	void replyFinished(QNetworkReply* aopReply);
};
#endif // MAINWINDOW_H
