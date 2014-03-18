#-------------------------------------------------
#
# Project created by QtCreator 2014-01-14T19:07:18
#
#-------------------------------------------------

QT       += opengl

TARGET = previewDDS
TEMPLATE = lib

CONFIG += plugins
CONFIG += dll

DEFINES += PREVIEWDDS_LIBRARY
DEFINES += NOMINMAX

SOURCES += previewdds.cpp \
    gl/gltexloaders.cpp \
    gl/dds/dds_api.cpp \
    gl/dds/Image.cpp \
    gl/dds/DirectDrawSurface.cpp \
    gl/dds/Stream.cpp \
    gl/dds/BlockDXT.cpp \
    gl/dds/ColorBlock.cpp

HEADERS += previewdds.h

INCLUDEPATH += "$(BOOSTPATH)"

include(../plugin_template.pri)
