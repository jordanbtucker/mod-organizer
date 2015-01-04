#-------------------------------------------------
#
# Project created by QtCreator 2012-10-31T12:47:48
#
#-------------------------------------------------

TARGET = uibase
TEMPLATE = lib

DEFINES += UIBASE_LIBRARY UIBASE_EXPORT
CONFIG += dll

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets qml declarative script quickwidgets
} else {
  QT += declarative script
}

!include(../LocalPaths.pri) {
  message("paths to required libraries need to be set up in LocalPaths.pri")
}

SOURCES += \
    utility.cpp \
    textviewer.cpp \
    finddialog.cpp \
    report.cpp \
    versioninfo.cpp \
    lineeditclear.cpp \
    mytree.cpp \
    installationtester.cpp \
    tutorialmanager.cpp \
    tutorialcontrol.cpp \
    tutorabledialog.cpp \
    scopeguard.cpp \
    pluginsetting.cpp \
    diagnosisreport.cpp \
    directorytree.cpp \
    iplugininstaller.cpp \
    guessedvalue.cpp \
    json.cpp \
    imodrepositorybridge.cpp \
    nxmurl.cpp \
    taskprogressmanager.cpp \
    questionboxmemory.cpp \
    modrepositoryfileinfo.cpp \
    sortabletreewidget.cpp \
    executableinfo.cpp

HEADERS +=\
    utility.h \
    textviewer.h \
    finddialog.h \
    report.h \
    iplugin.h \
    versioninfo.h \
    imoinfo.h \
    iplugintool.h \
    imodinterface.h \
    lineeditclear.h \
    dllimport.h \
    iplugininstaller.h \
    directorytree.h \
    mytree.h \
    iplugininstallersimple.h \
    iplugininstallercustom.h \
    installationtester.h \
    tutorialmanager.h \
    tutorialcontrol.h \
    tutorabledialog.h \
    iinstallationmanager.h \
    igameinfo.h \
    scopeguard.h \
    pluginsetting.h \
    iplugindiagnose.h \
    diagnosisreport.h \
    guessedvalue.h \
    ipluginproxy.h \
    json.h \
    imodrepositorybridge.h \
    idownloadmanager.h \
    nxmurl.h \
    taskprogressmanager.h \
    ipluginlist.h \
    ipluginmodpage.h \
    questionboxmemory.h \
    sortabletreewidget.h \
    imodlist.h \
    modrepositoryfileinfo.h \
    ipluginpreview.h \
    iplugingame.h \
    executableinfo.h

FORMS += \
    textviewer.ui \
    finddialog.ui \
    questionboxmemory.ui


CONFIG(debug, debug|release) {
  OUTDIR = $$OUT_PWD/debug
  DSTDIR = $$PWD/../../outputd
} else {
  OUTDIR = $$OUT_PWD/release
  DSTDIR = $$PWD/../../output
  SRCDIR = $$PWD
  QMAKE_CXXFLAGS += /Zi /GL
  QMAKE_LFLAGS += /DEBUG /LTCG /LARGEADDRESSAWARE /OPT:REF /OPT:ICF
}


OUTDIR ~= s,/,$$QMAKE_DIR_SEP,g
DSTDIR ~= s,/,$$QMAKE_DIR_SEP,g
SRCDIR ~= s,/,$$QMAKE_DIR_SEP,g

INCLUDEPATH += "$${BOOSTPATH}"

LIBS += -luser32 -lshell32 -lole32

QMAKE_POST_LINK += xcopy /y /s /I $$quote($$OUTDIR\\uibase.dll*) $$quote($$DSTDIR) $$escape_expand(\\n)
QMAKE_POST_LINK += xcopy /y /s /I $$quote($$OUTDIR\\uibase.pdb*) $$quote($$DSTDIR) $$escape_expand(\\n)
QMAKE_POST_LINK += xcopy /y /s /I $$quote($$OUTDIR\\uibase.lib*) $$quote($$SRCDIR\\..\\plugins\\helloworldcpp) $$escape_expand(\\n)
