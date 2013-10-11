INCLUDEPATH += ../../uibase

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

SRCDIR ~= s,/,$$QMAKE_DIR_SEP,g
DSTDIR ~= s,/,$$QMAKE_DIR_SEP,g

LIBS += -luibase -lshell32 -luser32

QMAKE_POST_LINK += xcopy /y /s /i $$quote($$SRCDIR\\$${TARGET}*.dll) $$quote($$DSTDIR)\\plugins $$escape_expand(\\n)
QMAKE_POST_LINK += xcopy /y /I $$quote($$SRCDIR\\$${TARGET}*.pdb) $$quote($$DSTDIR)\\plugins $$escape_expand(\\n)
