QT  += core gui
QT  += network
QT  += testlib
QT  += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	DialogClient_d.cpp \
	DialogFillData_d.cpp \
	DialogGetPassword_d.cpp \
	ManageClientsData_c.cpp \
	ManagePlaylists_c.cpp \
	MediaPlayer_c.cpp \
	SimpleCrypt/SimpleCryptQt.cpp \
	SpotifyAPI/SpotifyAPI_c.cpp \
	TestProbe/TestProbe_b.cpp \
	main.cpp \
	mainwindow.cpp

HEADERS += \
	DialogClient_d.h \
	DialogFillData_d.h \
	DialogGetPassword_d.h \
	ManageClientsData_c.h \
	ManagePlaylists_c.h \
	MediaPlayer_c.h \
	SimpleCrypt/SimpleCryptQt.h \
	SpotifyAPI/SpotifyAPI_c.h \
	SpotifyAPI/SpotifyUserSecrets_c.h \
	TestProbe/TestProbe_b.h \
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
