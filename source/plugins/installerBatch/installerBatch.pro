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



QMAKE_POST_LINK += pyuic4 -x $$PWD/modselection.ui -o $$PWD/modselection.py $$escape_expand(\\n)
# QMAKE_POST_LINK += pyrcc4 -o $$PWD/pyCfgResource_rc.py $$PWD/pyCfgResource.qrc $$escape_expand(\\n)
QMAKE_POST_LINK += copy $$WINPWD\\pyBatchInstaller.py $$quote($$DSTDIR)\\plugins $$escape_expand(\\n)
QMAKE_POST_LINK += xcopy /y /s /I $$WINPWD\\modselection.py $$quote($$DSTDIR)\\plugins\\data\\ $$escape_expand(\\n)
#QMAKE_POST_LINK += copy $$WINPWD\\pyCfgResource_rc.py $$quote($$DSTDIR)\\plugins\\data\\ $$escape_expand(\\n)


FORMS += \
    modselection.ui
