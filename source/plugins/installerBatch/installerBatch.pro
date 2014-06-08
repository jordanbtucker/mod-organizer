OTHER_FILES += \
    pyBatchInstaller.py

SOURCES += \
    dummy.cpp


CONFIG(debug, debug|release) {
	DSTDIR = $$PWD/../../../outputd
} else {
	DSTDIR = $$PWD/../../../output
}
WINPWD = $$PWD

DSTDIR ~= s,/,$$QMAKE_DIR_SEP,g
WINPWD ~= s,/,$$QMAKE_DIR_SEP,g



QMAKE_POST_LINK += pyuic4 -x $$quote($$PWD/modselection.ui) -o $$quote($$PWD/modselection.py) $$escape_expand(\\n)
QMAKE_POST_LINK += pyuic4 -x $$quote($$PWD/downloadselection.ui) -o $$quote($$PWD/downloadselection.py) $$escape_expand(\\n)
# QMAKE_POST_LINK += pyrcc4 -o $$PWD/pyCfgResource_rc.py $$PWD/pyCfgResource.qrc $$escape_expand(\\n)
QMAKE_POST_LINK += copy $$quote($$WINPWD\\pyBatchInstaller.py) $$quote($$DSTDIR\\plugins) $$escape_expand(\\n)
QMAKE_POST_LINK += xcopy /y /s /I $$quote($$WINPWD\\modselection.py) $$quote($$DSTDIR\\plugins\\data\\) $$escape_expand(\\n)
QMAKE_POST_LINK += xcopy /y /s /I $$quote($$WINPWD\\downloadselection.py) $$quote($$DSTDIR\\plugins\\data\\) $$escape_expand(\\n)
#QMAKE_POST_LINK += copy $$WINPWD\\pyCfgResource_rc.py $$quote($$DSTDIR)\\plugins\\data\\ $$escape_expand(\\n)


FORMS += \
    modselection.ui \
    downloadselection.ui
