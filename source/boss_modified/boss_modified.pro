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
    boss-api/BOSS-API.h \
#    boss-common/Updating/Updater.h \
#    boss-common/Output/Output.h \
#    boss-common/Parsing/Grammar.h \
#    boss-common/Support/Helpers.h \
#    boss-common/Support/Logger.h \
#    boss-common/Support/ModFormat.h \
#    boss-common/Support/Platform.h \
#    boss-common/Support/Types.h \
#    boss-common/Support/VersionRegex.h \
#    boss-common/Common/Classes.h \
#    boss-common/Common/DllDef.h \
#    boss-common/Common/Error.h \
#    boss-common/Common/Game.h \
#    boss-common/Common/Globals.h


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
