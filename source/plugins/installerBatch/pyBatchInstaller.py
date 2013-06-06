import csv
import time
import sys
import os
from PyQt4 import QtCore, QtGui


class DownloadSelectionDialog(QtGui.QDialog):
    def __init__(self, parent = None):
        super(DownloadSelectionDialog, self).__init__(parent)

        from downloadselection import Ui_DownloadSelectionDialog
        self.__ui = Ui_DownloadSelectionDialog()
        self.__ui.setupUi(self)
        self.setWindowFlags(self.windowFlags() & ~QtCore.Qt.WindowContextHelpButtonHint)
        self.__ui.choicesList.currentItemChanged.connect(self.itemSelected)
        self.__ui.choicesList.item(0).setData(QtCore.Qt.UserRole, -1)

    def setOldName(self, name):
        self.__ui.oldNameLabel.setText(name)

    def itemSelected(self, newItem, oldItem):
        self.__ui.descriptionLabel.setText(newItem.data(QtCore.Qt.UserRole + 1).toString())

    def addRepositoryInfo(self, name, fileID, description):
        item = QtGui.QListWidgetItem(name)
        item.setData(QtCore.Qt.UserRole, fileID)
        item.setData(QtCore.Qt.UserRole + 1, description)
        self.__ui.choicesList.addItem(item)

    def selectedFileID(self):
        return self.__ui.choicesList.currentItem().data(QtCore.Qt.UserRole).toInt()[0]


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
        self.__nexusBridge = mobase.ModRepositoryBridge()
        self.__parentWidget = None
        self.__remaining = 0
        self.__requestedFiles = {}
        self.__downloadedFiles = []
        self.__fail = False
        self.__nexusBridge.onFilesAvailable(self.onFilesAvailable)
        self.__nexusBridge.onRequestFailed(self.onRequestFailed)

        temp = self.__organizer.downloadManager()
        if  not QtCore.QObject.connect(mobase.toPyQt(temp),  QtCore.SIGNAL("downloadComplete(int)"),  self.onDownloadComplete) or \
            not QtCore.QObject.connect(mobase.toPyQt(temp),  QtCore.SIGNAL("downloadFailed(int)"),  self.onDownloadFailed):
                print("failed to connect to signal")
                return False
        return True

    def onFilesAvailable(self,  modID,  userData,  resultData):
        fileID = -1
        for file in resultData:
            if file["uri"] == userData:
                fileID = file["fileID"]

        if fileID == -1:
            selDialog = DownloadSelectionDialog(self.__parentWidget)
            selDialog.setOldName(userData)
            for file in resultData:
                selDialog.addRepositoryInfo(file["name"], int(file["fileID"]), file["description"])
            selDialog.exec_()
            fileID = selDialog.selectedFileID()

        downloadID = self.__organizer.downloadManager().startDownloadNexusFile(modID,  fileID)
        self.__requestedFiles[downloadID] = userData
        self.__remaining -= 1

    def onRequestFailed(self,  modID,  userData,  errorMessage):
        QtGui.QMessageBox.critical(self.__parentWidget,  'Error',  errorMessage)
        self.__remaining -= 1

    def onDownloadComplete(self,  downloadID):
        self.__organizer.installMod(self.__organizer.downloadsPath() + "/" + self.__requestedFiles[downloadID])
        del self.__requestedFiles[downloadID]
        self.__remaining -= 1

    def onDownloadFailed(self,  downloadID):
        print("download " + downloadID + " failed")
        del self.__requestedFiles[downloadID]
        self.__remaining -= 1

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
        self.__parentWidget = widget

    def priority(self):
        return 50

    def isManualInstaller(self):
        return False

    def isArchiveSupported(self,  name):
        return True

    def supportedExtensions(self):
        return [ "csv" ]

    def cancelProgress(self):
        self.__remaining = 0

    def install(self,  modNameGuessed,  archiveName,  version,  modid):
        self.__remaining = 0
        self.__requestedFiles = {}
        self.__downloadedFiles = []

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

                downloadsPath = self.__organizer.downloadsPath()

                for sel in choices:
                    if not os.path.exists(downloadsPath + "/" + sel["file_installed_name"]):
                        print("start download " + sel["file_installed_name"])
                        self.__nexusBridge.requestFiles(int(sel["mod_id"]),  sel["file_installed_name"])
                        self.__remaining += 1
                    else:
                        print("file already downloaded: " + sel["file_installed_name"])
                        self.__downloadedFiles.append(sel["file_installed_name"])

        total = self.__remaining
        progress = QtGui.QProgressDialog("Initiating download for missing files",  "Cancel",  0,  total,  self.__parentWidget)
        progress.canceled.connect(self.cancelProgress)
        progress.show()
        while self.__remaining > 0:
            progress.setValue(total - self.__remaining)
            QtCore.QCoreApplication.processEvents()
            time.sleep(0.1)

        progress.setValue(0)
        self.__remaining = len(self.__downloadedFiles) + total
        progress.setMaximum(self.__remaining)
        for file in self.__downloadedFiles:
            self.__organizer.installMod(self.__organizer.downloadsPath() + "\\" + file)
            self.__remaining -= 1
            progress.setValue(total - self.__remaining)

        while self.__remaining > 0:
            QtCore.QCoreApplication.processEvents()
            progress.setValue(total - self.__remaining)
            time.sleep(0.5)

        progress.close()

        return mobase.InstallResult.success

def createPlugin():
        return InstallerBatch()
