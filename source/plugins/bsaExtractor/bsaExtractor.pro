#-------------------------------------------------
#
# Project created by QtCreator 2014-05-19T20:05:08
#
#-------------------------------------------------

TARGET = bsaExtractor
TEMPLATE = lib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += plugins
CONFIG += dll

DEFINES += BSAEXTRACTOR_LIBRARY

SOURCES += bsaextractor.cpp

HEADERS += bsaextractor.h

OTHER_FILES += \
    bsaextractor.json

include(../plugin_template.pri)

CONFIG(debug, debug|release) {
  LIBS += -L$$OUT_PWD/../../bsatk/debug
  PRE_TARGETDEPS +=  $$OUT_PWD/../../bsatk/debug/bsatk.lib
} else {
  LIBS += -L$$OUT_PWD/../../bsatk/release
  PRE_TARGETDEPS +=  $$OUT_PWD/../../bsatk/release/bsatk.lib
}


INCLUDEPATH += ../../bsatk "$(BOOSTPATH)"

LIBS += -L"$(BOOSTPATH)/stage/lib" -lbsatk
LIBS += -L"$(ZLIBPATH)/build" -lzlibstatic
OTHER_FILES += \
    bsaextractor.json
