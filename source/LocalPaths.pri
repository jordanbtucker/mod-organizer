BOOSTPATH=E:/boost/boost_1_55_0
LOOTPATH=C:/Users/Tannin/Documents/Projects/loot_api
PYTHONPATH=E:/Python278
SEVENZIPPATH=E:/Documents/Projects/7zip
ZLIBPATH=E:/Documents/Projects/zlib-1.2.7


CONFIG(debug, debug|release) {
  SRCDIR = "$$OUT_PWD/debug"
  DSTDIR = "$$PWD/../outputd"
} else {
  SRCDIR = "$$OUT_PWD/release"
  DSTDIR = "$$PWD/../output"
}

SRCDIR ~= s,/,$$QMAKE_DIR_SEP,g
DSTDIR ~= s,/,$$QMAKE_DIR_SEP,g
