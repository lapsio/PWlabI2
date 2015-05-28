TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -march=corei7

LIBS += -lsfml-graphics -lsfml-window -lsfml-system

SOURCES += main.cpp \
    src/map.cpp \
    src/object.cpp \
    src/physx.cpp \
    src/PointXY.cpp \
    src/tests.cpp \
    src/VectorXY.cpp \
    src/utils.cpp \
    src/GameDomain.cpp \
    src/session.cpp \
    src/gfx.cpp \
    src/engine.cpp \
    drivers/src/Actor.cpp \
    drivers/src/EngineStart.cpp \
    drivers/src/Intro.cpp \
    drivers/src/PlayerController.cpp \
    drivers/src/Static.cpp \
    drivers/src/Hud.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    include/map.h \
    include/misc.h \
    include/object.h \
    include/physx.h \
    include/PointXY.h \
    include/tests.h \
    include/VectorXY.h \
    include/defines.h \
    include/utils.h \
    include/GameDomain.h \
    include/session.h \
    include/gfx.h \
    include/engine.h \
    drivers/include/Actor.h \
    drivers/include/EngineStart.h \
    drivers/include/Intro.h \
    drivers/include/PlayerController.h \
    drivers/include/Static.h \
    drivers/include/Hud.h

