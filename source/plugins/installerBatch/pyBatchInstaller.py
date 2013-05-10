import csv
import time
import sys
from PyQt4 import QtCore, QtGui


class ModSelectionDialog(QtGui.QDialog):
    def __init__(self, parent = None):
        super(ModSelectionDialog, self).__init__(parent)

        from modselection import Ui_ModSelectionDialog
        self.__ui = Ui_ModSelectionDialog()
        self.__ui.setupUi(self)
        self.setWindowFlags(self.windowFlags() & ~QtCore.Qt.WindowContextHelpButtonHint)

    def addChoice(self, name, userData):
        item = QtGui.QListWidgetItem(name)
        item.setData(QtCore.Qt.UserRole, userData)
        item.setFlags(item.flags() | QtCore.Qt.ItemIsUserCheckable)
        item.setCheckState(QtCore.Qt.Checked)
        self.__ui.modList.addItem(item)

    def choices(self):
        result = []
        for i in range(self.__ui.modList.count()):
            if self.__ui.modList.item(i).checkState() == QtCore.Qt.Checked:
                result.append(self.__ui.modList.item(i).data(QtCore.Qt.UserRole))
        return result

class InstallerBatch(mobase.IPluginInstallerCustom):
    def init(self, organizer):
        self.__organizer = organizer
        self.__nexusBridge = self.__organizer.createNexusBridge()
        self.__parentWidget = None
        self.__remaining = 0
        self.__fail = False

        if not QtCore.QObject.connect(mobase.toPyQt(self.__nexusBridge),
                QtCore.SIGNAL('filesAvailable(int, QVariant, PyQt_PyObject)'),  self.nxmFilesAvailable) or \
           not QtCore.QObject.connect(mobase.toPyQt(self.__nexusBridge),
                QtCore.SIGNAL('requestFailed(int, QVariant, int, QString)'), self.nxmFilesAvailable) or \
           not QtCore.QObject.connect(mobase.toPyQt(self.__nexusBridge),
                QtCore.SIGNAL('bla(QString)'), self.blaSlot):
            print("failed to connect signals")
            return False
        return True

    def name(self):
        return "Batch Installer"

    def author(self):
        return "Tannin"

    def description(self):
        return "Install multiple mods from a csv list"

    def version(self):
        return mobase.VersionInfo(1, 0, 0, mobase.ReleaseType.beta)

    def isActive(self):
        return True

    def settings(self):
        return []

    def setParentWidget(self, widget):
        print("have parent widget")
        self.__parentWidget = widget

    def priority(self):
        return 50

    def isManualInstaller(self):
        return True

    def isArchiveSupported(self,  name):
        return True

    def supportedExtensions(self):
        return [ "csv" ]

    def nxmFilesAvailable(self,  modID, userData, resultData):
        QtGui.QMessageBox.information(self.__parentWidget, 'Files', 'files available: ' + str(userData.toString()))

        QtGui.QMessageBox.information(self.__parentWidget, 'Files', 'res data: ' + str(resultData))

#        for var in resultData.toList():
#            fileInfo = var.toMap()
#            QtGui.QMessageBox.information(self.__parentWidget, 'Files', fileInfo["uri"].toString())

        self.__remaining -= 1

    def nxmRequestFailed(self,  modID, userData,  requestID,  errorMessage):
        QtGui.QMessageBox.warning(None,  'Error',  'Failed to retrieve file information')

    def blaSlot(self,  var):
        QtGui.QMessageBox.warning(None,  'Bla',  'bla')

    def install(self,  modNameGuessed,  archiveName):
        app = QtGui.QApplication(sys.argv)
        with open(archiveName, 'rb') as csvfile:
            selDialog = ModSelectionDialog(self.__parentWidget)

            modReader = csv.DictReader(csvfile)
            modNames = []
            for row in modReader:
                modNames.append(row["mod_installed_name"])
                selDialog.addChoice(row["mod_installed_name"], (row,))
            if selDialog.exec_() == QtGui.QDialog.Accepted:
                # row data was wrapped in a tuple
                choices = [ sel.toPyObject()[0] for sel in selDialog.choices() ]
                for sel in choices:
                    self.__nexusBridge.requestFiles(int(sel["mod_id"]),  QtCore.QString("blabla"))
                    self.__remaining += 1

        while self.__remaining > 0:
            QtCore.QCoreApplication.processEvents()
            time.sleep(0.5)


        return mobase.InstallResult.canceled

def createPlugin():
        return InstallerBatch()
