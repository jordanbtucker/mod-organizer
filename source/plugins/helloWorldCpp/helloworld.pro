TARGET = helloWorld
TEMPLATE = lib

contains(QT_VERSION, "^5.*") {
  QT += widgets
}

CONFIG += plugins
CONFIG += dll

DEFINES += HELLOWORLD_LIBRARY

SOURCES += helloworld.cpp

HEADERS += helloworld.h

include(../plugin_template.pri)

OTHER_FILES += \
    helloworld.json

LIBS += -L$$PWD
