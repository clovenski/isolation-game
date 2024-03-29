#-------------------------------------------------
#
# Project created by QtCreator 2019-06-06T23:34:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = isolation
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
    src/aipiece.cpp \
    src/confirmform.cpp \
    src/isolationform.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/playerpiece.cpp \
    ../engine/src/Engine.cpp \
    ../engine/src/Minimax.cpp \
    ../engine/src/State.cpp \
    src/titleform.cpp

HEADERS += \
    include/aipiece.h \
    include/confirmform.h \
    include/isolationform.h \
    include/mainwindow.h \
    include/playerpiece.h \
    ../engine/include/Engine.h \
    ../engine/include/Minimax.h \
    ../engine/include/State.h \
    include/titleform.h \

FORMS += \
    ui/confirmform.ui \
    ui/isolationform.ui \
    ui/mainwindow.ui \
    ui/titleform.ui

INCLUDEPATH += \
    $$_PRO_FILE_PWD_/include/ \
    $$_PRO_FILE_PWD_/../engine/include/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
