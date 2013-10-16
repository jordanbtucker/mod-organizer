SOURCES += \
    dummy.cpp

OTHER_FILES += \
    installerBCF.py


CONFIG(debug, debug|release) {
  DSTDIR = $$PWD/../../../outputd
} else {
  DSTDIR = $$PWD/../../../output
}
WINPWD = $$PWD

DSTDIR ~= s,/,$$QMAKE_DIR_SEP,g
WINPWD ~= s,/,$$QMAKE_DIR_SEP,g

QMAKE_POST_LINK += copy $$WINPWD\\installerBCF.py $$quote($$DSTDIR)\\plugins $$escape_expand(\\n)
