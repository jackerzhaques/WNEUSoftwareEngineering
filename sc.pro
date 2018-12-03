#-------------------------------------------------
#
# Project created by QtCreator 2018-11-01T23:45:25
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sc
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
        main.cpp \
        mainwindow.cpp \
    camera.cpp \
    musicplayer.cpp \
    levelmanager.cpp \
    entity.cpp \
    player.cpp \
    npc.cpp \
    projectile.cpp \
    terrain.cpp \
    doodad.cpp \
    destructible.cpp \
    wall.cpp \
    item.cpp \
    ability.cpp \
    characterinterface.cpp \
    game.cpp \
    spritesheet.cpp \
    scene.cpp \
    collisiondetection.cpp \
    floatingtext.cpp

HEADERS += \
        mainwindow.h \
    camera.h \
    musicplayer.h \
    levelmanager.h \
    entity.h \
    player.h \
    npc.h \
    projectile.h \
    terrain.h \
    doodad.h \
    destructible.h \
    wall.h \
    item.h \
    ability.h \
    characterinterface.h \
    game.h \
    spritesheet.h \
    scene.h \
    collisiondetection.h \
    floatingtext.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
