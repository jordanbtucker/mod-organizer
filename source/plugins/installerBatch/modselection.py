# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'C:/Users/Tannin/Documents/Projects/modorganizer-sf/source/plugins/installerBatch/modselection.ui'
#
# Created: Thu May 23 18:58:30 2013
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

class Ui_ModSelectionDialog(object):
    def setupUi(self, ModSelectionDialog):
        ModSelectionDialog.setObjectName(_fromUtf8("ModSelectionDialog"))
        ModSelectionDialog.resize(461, 362)
        self.verticalLayout = QtGui.QVBoxLayout(ModSelectionDialog)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.modList = QtGui.QListWidget(ModSelectionDialog)
        self.modList.setObjectName(_fromUtf8("modList"))
        self.verticalLayout.addWidget(self.modList)
        self.buttonBox = QtGui.QDialogButtonBox(ModSelectionDialog)
        self.buttonBox.setOrientation(QtCore.Qt.Horizontal)
        self.buttonBox.setStandardButtons(QtGui.QDialogButtonBox.Cancel|QtGui.QDialogButtonBox.Ok)
        self.buttonBox.setObjectName(_fromUtf8("buttonBox"))
        self.verticalLayout.addWidget(self.buttonBox)

        self.retranslateUi(ModSelectionDialog)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("accepted()")), ModSelectionDialog.accept)
        QtCore.QObject.connect(self.buttonBox, QtCore.SIGNAL(_fromUtf8("rejected()")), ModSelectionDialog.reject)
        QtCore.QMetaObject.connectSlotsByName(ModSelectionDialog)

    def retranslateUi(self, ModSelectionDialog):
        ModSelectionDialog.setWindowTitle(_translate("ModSelectionDialog", "Select Mods", None))


if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    ModSelectionDialog = QtGui.QDialog()
    ui = Ui_ModSelectionDialog()
    ui.setupUi(ModSelectionDialog)
    ModSelectionDialog.show()
    sys.exit(app.exec_())

