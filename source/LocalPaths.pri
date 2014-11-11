BOOSTPATH=E:/boost/boost_1_56_0
CURLPATH=E:/Documents/Projects/curl
LOOTPATH=C:/Users/Tannin/Documents/Projects/loot_api
PYTHONPATH=E:/Python278
SEVENZIPPATH=E:/Documents/Projects/7zip
UTF8CPPPATH=E:/Documents/Projects/utf8
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
