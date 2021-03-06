#-------------------------------------------------
#
# Project created by QtCreator 2019-01-26T22:27:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Toolkit
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    LogClass.cpp \
        main.cpp \
        mainwindow.cpp \
    configclass.cpp \
    timeclass.cpp \
    invokeclass.cpp \
    dialog/basedialog.cpp \
    dialog/newprojectdialog.cpp \
    dialog/projectmanagerdialog.cpp \
    dialog/showpathdialog.cpp \
    dialog/projectdetaildialog.cpp \
    dialog/newsvnpathdialog.cpp \
    dialog/newscriptdialog.cpp \
    dialog/scriptoperatedialog.cpp \
    dialog/globalscrtipmanagerdialog.cpp

HEADERS += \
    LogClass.h \
        mainwindow.h \
    configclass.h \
    stdafx.h \
    timeclass.h \
    invokeclass.h \
    errornum.h \
    dialog/basedialog.h \
    dialog/newprojectdialog.h \
    dialog/projectmanagerdialog.h \
    dialog/showpathdialog.h \
    dialog/projectdetaildialog.h \
    dialog/newsvnpathdialog.h \
    dialog/newscriptdialog.h \
    dialog/dialogcommondef.h \
    dialog/scriptoperatedialog.h \
    dialog/globalscrtipmanagerdialog.h

FORMS += \
        mainwindow.ui \
    dialog/basedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/resource.qrc
