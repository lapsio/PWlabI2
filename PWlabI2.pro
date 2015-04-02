TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    src/gc.cpp \
    src/map.cpp \
    src/object.cpp \
    src/physx.cpp \
    src/PointXY.cpp \
    src/tests.cpp \
    src/VectorXY.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    include/gc.h \
    include/map.h \
    include/misc.h \
    include/object.h \
    include/physx.h \
    include/PointXY.h \
    include/tests.h \
    include/VectorXY.h \
    include/defines.h

