#-------------------------------------------------
#
# Project created by QtCreator 2013-08-22T22:50:32
#
#-------------------------------------------------

QT       -= core gui

TARGET = BossDummy
TEMPLATE = lib

DEFINES += BOSSDUMMY_LIBRARY

SOURCES += bossdummy.cpp

HEADERS += bossdummy.h


CONFIG(debug, debug|release) {
  SRCDIR = $$OUT_PWD/debug
  DSTDIR = $$PWD/../../outputd
} else {
  SRCDIR = $$OUT_PWD/release
  DSTDIR = $$PWD/../../output
}

SRCDIR ~= s,/,$$QMAKE_DIR_SEP,g
DSTDIR ~= s,/,$$QMAKE_DIR_SEP,g


QMAKE_POST_LINK += $$quote($$PWD\\..\\NCC\\publish.bat) $$escape_expand(\\n)
QMAKE_POST_LINK += copy $$quote($$SRCDIR\\BossDummy.dll) $$quote($$DSTDIR\\NCC\\GameModes\\data\\boss32.dll) $$escape_expand(\\n)
QMAKE_POST_LINK += copy $$quote($$SRCDIR\\BossDummy.dll) $$quote($$DSTDIR\\NCC\\GameModes\\data\\boss64.dll) $$escape_expand(\\n)
