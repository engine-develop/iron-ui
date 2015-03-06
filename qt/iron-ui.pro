#-------------------------------------------------
#
# Project created by QtCreator 2012-04-16T14:33:05
#
#-------------------------------------------------

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iron-ui
TEMPLATE = app

INCLUDEPATH += ../../../iron/ext/chaos-pp-master
INCLUDEPATH += ../../../iron/build/iron/include
LIBS += -L../../../iron/build/iron/lib -liron

win32 {
    LIBS += -lsetupapi
}

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += \
    ../src/connection.cpp \
    ../src/main.cpp \
    ../src/mainwindow.cpp \
    ../src/port.cpp \
    ../src/typestore.cpp \
    ../src/node.cpp \
    ../src/graph.cpp

HEADERS += \
    ../src/connection.hpp \
    ../src/mainwindow.hpp \
    ../src/port.hpp \
    ../src/typestore.hpp \
    ../src/node.hpp \
    ../src/graph.hpp \
    ../src/utility.hpp

FORMS +=

RESOURCES += \
    resources.qrc
