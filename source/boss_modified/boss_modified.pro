QT       -= core gui

TARGET = boss
TEMPLATE = lib


SOURCES += \
    boss-api/BOSS-API.cpp \
    boss-common/Updating/Updater.cpp \
    boss-common/Output/Output.cpp \
    boss-common/Parsing/Grammar.cpp \
    boss-common/Support/Helpers.cpp \
    boss-common/Support/Logger.cpp \
    boss-common/Support/ModFormat.cpp \
    boss-common/Support/VersionRegex.cpp \
    boss-common/Common/Classes.cpp \
    boss-common/Common/Error.cpp \
    boss-common/Common/Game.cpp \
    boss-common/Common/Globals.cpp

HEADERS  += \
    boss-api/BOSS-API.h


INCLUDEPATH += "$$PWD" "$$PWD/boss-common" "$(BOOSTPATH)" "$(CURLPATH)/include" "$(UTF8CPPPATH)"

LIBS += -L"$(BOOSTPATH)/stage/lib"

LIBS += -L"$(CURLPATH)/lib/Release" -llibcurl_a

LIBS += -ladvapi32 -lVersion -lshell32

QMAKE_CXXFLAGS -= -Zc:wchar_t-
QMAKE_CXXFLAGS += -Zc:wchar_t /Zi /nologo /GL

QMAKE_LFLAGS +=  /NOLOGO /LTCG /MANIFESTUAC:\"level=\'asInvoker\' uiAccess=\'false\'\" /LARGEADDRESSAWARE /OPT:REF /OPT:ICF /TLBID:1 /DYNAMICBASE /NXCOMPAT /MACHINE:X86 /ERRORREPORT:QUEUE

DEFINES += _CRT_SECURE_NO_WARNINGS
DEFINES += BOSS_EXPORT
DEFINES += CURL_STATICLIB
DEFINES += NDEBUG

QMAKE_CFLAGS_WARN_ON -= -W3
QMAKE_CFLAGS_WARN_ON += -W1



CONFIG(debug, debug|release) {
  SRCDIR = $$OUT_PWD/debug
  DSTDIR = $$PWD/../../outputd
} else {
  SRCDIR = $$OUT_PWD/release
  DSTDIR = $$PWD/../../output
}

SRCDIR ~= s,/,$$QMAKE_DIR_SEP,g
DSTDIR ~= s,/,$$QMAKE_DIR_SEP,g

QMAKE_POST_LINK += xcopy /y /I $$quote($$SRCDIR\\boss*.dll) $$quote($$DSTDIR\\dlls) $$escape_expand(\\n)
QMAKE_POST_LINK += xcopy /y /I $$quote($$SRCDIR\\boss*.pdb) $$quote($$DSTDIR\\dlls) $$escape_expand(\\n)
