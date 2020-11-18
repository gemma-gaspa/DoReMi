QT  += core gui
QT  += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	DialogClient_d.cpp \
	DialogFillData_d.cpp \
	DialogGetPassword_d.cpp \
	GaspaSpotifyAPI_c.cpp \
	ManageUserData_c.cpp \
	SimpleCryptQt.cpp \
	main.cpp \
	mainwindow.cpp

HEADERS += \
	DialogClient_d.h \
	DialogFillData_d.h \
	DialogGetPassword_d.h \
	GaspaSpotifyAPI_c.h \
	ManageUserData_c.h \
	SimpleCryptQt.h \
	SpotifyUserSecrets_c.h \
	mainwindow.h

FORMS += \
	DialogClient_d.ui \
	DialogFillData_d.ui \
	DialogGetPassword_d.ui \
	mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
