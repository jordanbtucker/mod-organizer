import os
import sys
import cPickle
import ConfigParser
from PyQt4 import QtCore, QtGui
from PyQt4.QtCore import Qt, pyqtWrapperType, pyqtSlot, pyqtSignal
from PyQt4.QtGui import QDialog, QMessageBox


# see InstallerConverter in bosh.py
# see InstallerConverter_Create in basher.py


if not "mobase" in sys.modules:
    import mock_mobase as mobase


class InstallerBCF(mobase.IPluginInstallerCustom):
    def init(self, organizer):
        self.__organizer = organizer
        self.__parentWidget = None
        return True

    def name(self):
        return "BCF Installer"

    def author(self):
        return "Tannin"

    def description(self):
        return "Plugin to introduce support of bcf files"

    def version(self):
        return mobase.VersionInfo(0, 0, 1, mobase.ReleaseType.prealpha)

    def isActive(self):
        return True

    def settings(self):
        return []

    def setParentWidget(self, widget):
        self.__parentWidget = widget

    def priority(self):
        return 1

    def isManualInstaller(self):
        return False

    def isArchiveSupported(self, archiveName):
        return True

    def supportedExtensions(self):
        return [ 'bcf' ]

    def install(self, modNameGuessed, archiveName, version, modid):
        pass
    

def createPlugin():
        return InstallerBCF()
