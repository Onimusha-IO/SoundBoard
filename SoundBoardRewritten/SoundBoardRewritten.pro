#-------------------------------------------------
#
# Project created by QtCreator 2018-03-08T17:33:52
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SoundBoardRewritten
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    player.cpp \
    scene.cpp

HEADERS  += widget.h \
    player.h \
    scene.h

FORMS    += widget.ui \
    player.ui \
    scene.ui

RESOURCES += \
    src.qrc
