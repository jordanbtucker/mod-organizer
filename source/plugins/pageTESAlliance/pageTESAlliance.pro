#-------------------------------------------------
#
# Project created by QtCreator 2013-09-14T14:08:18
#
#-------------------------------------------------


TARGET = pageTESAlliance
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

#CONFIG(release, debug|release) {
#  QMAKE_CXXFLAGS += /Zi
#  QMAKE_LFLAGS += /DEBUG
#}

INCLUDEPATH += "$(BOOSTPATH)"

DEFINES += PAGETESALLIANCE_LIBRARY

SOURCES += pagetesalliance.cpp

HEADERS += pagetesalliance.h

OTHER_FILES +=

include(../plugin_template.pri)

