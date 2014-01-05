QT       -= core gui

TARGET = proxy

TEMPLATE = lib

DEFINES += OMO_DLL_LIBRARY

OTHER_FILES +=

SOURCES += \
    steam_api.dll.cpp


DEFINES -= UNICODE
DEFINES += _CRT_SECURE_NO_WARNINGS _WINDLL


CONFIG(debug, debug|release) {
  SRCDIR = $$OUT_PWD/debug
  DSTDIR = $$PWD/../../outputd
} else {
  SRCDIR = $$OUT_PWD/release
  DSTDIR = $$PWD/../../output
}

SRCDIR ~= s,/,$$QMAKE_DIR_SEP,g
DSTDIR ~= s,/,$$QMAKE_DIR_SEP,g

QMAKE_POST_LINK += xcopy /y /I $$quote($$SRCDIR\\proxy.dll*) $$quote($$DSTDIR) $$escape_expand(\\n)
