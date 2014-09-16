#-------------------------------------------------
#
# Project created by QtCreator 2014-05-12T22:51:24
#
#-------------------------------------------------

QT       += widgets core gui

TARGET = e5installer
TEMPLATE = app


SOURCES += main.cpp \
    CInstallThread.cpp \
    installdialog.cpp

HEADERS  += \
    CInstallThread.h \
    installdialog.h

FORMS += \
    installdialog.ui

OTHER_FILES +=

RESOURCES += \
    res.qrc
