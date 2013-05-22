# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'C:/Users/Tannin/Documents/Projects/modorganizer-sf/source/plugins/installerBatch/downloadselection.ui'
#
# Created: Fri May 17 20:32:53 2013
#      by: PyQt4 UI code generator 4.10
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_DownloadSelectionDialog(object):
    def setupUi(self, DownloadSelectionDialog):
        DownloadSelectionDialog.setObjectName(_fromUtf8("DownloadSelectionDialog"))
        DownloadSelectionDialog.resize(465, 337)
        self.verticalLayout = QtGui.QVBoxLayout(DownloadSelectionDialog)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.label = QtGui.QLabel(DownloadSelectionDialog)
        self.label.setWordWrap(True)
        self.label.setObjectName(_fromUtf8("label"))
        self.verticalLayout.addWidget(self.label)
        self.oldNameLabel = QtGui.QLabel(DownloadSelectionDialog)
        self.oldNameLabel.setText(_fromUtf8("Old Name"))
        self.oldNameLabel.setObjectName(_fromUtf8("oldNameLabel"))
        self.verticalLayout.addWidget(self.oldNameLabel)
        self.choicesList = QtGui.QListWidget(DownloadSelectionDialog)
        self.choicesList.setObjectName(_fromUtf8("choicesList"))
        item = QtGui.QListWidgetItem()
        self.choicesList.addItem(item)
        self.verticalLayout.addWidget(self.choicesList)
        self.descriptionLabel = QtGui.QLabel(DownloadSelectionDialog)
        self.descriptionLabel.setText(_fromUtf8("Description"))
        self.descriptionLabel.setObjectName(_fromUtf8("descriptionLabel"))
        self.verticalLayout.addWidget(self.descriptionLabel)
        self.buttonBox = QtGui.QDialogButtonBox(DownloadSelectionDialog)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName(_fromUtf8("buttonBox"))
        self.verticalLayout.addWidget(self.buttonBox)

        self.retranslateUi(DownloadSelectionDialog)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("accepted()")), DownloadSelectionDialog.accept)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("rejected()")), DownloadSelectionDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(DownloadSelectionDialog)

    def retranslateUi(self, DownloadSelectionDialog):
        DownloadSelectionDialog.setWindowTitle(_translate("DownloadSelectionDialog", "Dialog", None))
        self.label.setText(_translate("DownloadSelectionDialog", "A file specified in the batch installer is no longer available. Please choose a mod archive to download instead.", None))
        __sortingEnabled = self.choicesList.isSortingEnabled()
        self.choicesList.setSortingEnabled(False)
        item = self.choicesList.item(0)
        item.setText(_translate("DownloadSelectionDialog", "<Skip>", None))
        item.setToolTip(_translate("DownloadSelectionDialog", "Don\'t install this mod", None))
        self.choicesList.setSortingEnabled(__sortingEnabled)


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    DownloadSelectionDialog = QtGui.QDialog()
    ui = Ui_DownloadSelectionDialog()
    ui.setupUi(DownloadSelectionDialog)
    DownloadSelectionDialog.show()
    sys.exit(app.exec_())

