TEMPLATE = subdirs

SUBDIRS += \
    iniEditor \
    NMMImport \
    installerManual \
    installerNCC \
    installerBAIN \
    installerFomod \
    installerQuick \
    installerBundle \
    proxyPython \
    diagnoseBasic \
    pyniEdit \
    checkFNIS \
    previewBase \
    bsaExtractor \
    gameSkyrim


plugins.depends = gameSkyrim
