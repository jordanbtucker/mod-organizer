CONFIG(debug, debug|release) {
  SRCDIR = $$OUT_PWD/debug
  DSTDIR = $$PWD/../../outputd
  LIBS += -L$$OUT_PWD/../../uibase/debug
} else {
  SRCDIR = $$OUT_PWD/release
  DSTDIR = $$PWD/../../output
  LIBS += -L$$OUT_PWD/../../uibase/release
  QMAKE_CXXFLAGS += /Zi
  QMAKE_LFLAGS += /DEBUG
}


!include(../LocalPaths.pri) {
  message("paths to required libraries need to be set up in LocalPaths.pri")
}

SRCDIR ~= s,/,$$QMAKE_DIR_SEP,g
DSTDIR ~= s,/,$$QMAKE_DIR_SEP,g

TRANSLATIONS = $${TARGET}_en.ts \
               $${TARGET}_de.ts \
               $${TARGET}_es.ts \
               $${TARGET}_fr.ts \
               $${TARGET}_zh_TW.ts \
               $${TARGET}_zh_CN.ts \
               $${TARGET}_cs.ts \
               $${TARGET}_tr.ts \
               $${TARGET}_ru.ts

INCLUDEPATH += $${PWD}/../uibase
LIBS += -luibase -lshell32 -luser32

QMAKE_POST_LINK += xcopy /y /s /i $$quote($$SRCDIR\\$${TARGET}*.dll) $$quote($$DSTDIR)\\plugins $$escape_expand(\\n)
QMAKE_POST_LINK += xcopy /y /I $$quote($$SRCDIR\\$${TARGET}*.pdb) $$quote($$DSTDIR)\\plugins $$escape_expand(\\n)
