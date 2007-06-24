######################################################################
# Automatically generated by qmake (2.01a) sab 26. mag 12:53:10 2007
######################################################################

TEMPLATE = app
TARGET = tractor
DEPENDPATH += . pages
INCLUDEPATH += .
QT += network xml

# Input
HEADERS += mainwindow.h \
           options.h \
           ttoolbutton.h \
           pages/downloads.h \
           pages/search.h \
           pages/servers.h \
           pages/shared.h \
           pages/options_pages.h \
           TClient.h \
           TTransfer.h \
           TParser.h \
           TServer.h \
           TSettings.h \
           TClientSocket.h \
           md5.h \
           TShared.h \
           TXml.h \
           info_structs.h
SOURCES += main.cpp \
           mainwindow.cpp \
           options.cpp \
           ttoolbutton.cpp \
           pages/downloads.cpp \
           pages/search.cpp \
           pages/servers.cpp \
           pages/shared.cpp \
           pages/options_pages.cpp \
           TClient.cpp \
           TTransfer.cpp \
           TParser.cpp \
           TServer.cpp \
           TSettings.cpp \
           TClientSocket.cpp \
           md5.cpp \
           TShared.cpp \
           TXml.cpp
RESOURCES += icons.qrc

win32 {
	RC_FILE = winicon.rc
}
