import os
import sys
import json
import ConfigParser
from PyQt4 import QtCore, QtGui
from PyQt4.QtCore import Qt, pyqtWrapperType, pyqtSlot, pyqtSignal
from PyQt4.QtGui import QDialog, QHeaderView, QMessageBox, QColorDialog, QPalette, QTreeWidgetItem,\
QComboBox, QPushButton, QDoubleSpinBox, QHBoxLayout, QWidget, QSlider, QSpinBox, QLineEdit
pyqt5 = False

# qt5
#from PyQt5 import QtCore, QtGui, QtWidgets
#from PyQt5.QtCore import Qt, pyqtWrapperType, pyqtSlot, pyqtSignal
#from PyQt5.QtWidget import QDialog, QHeaderView, QMessageBox, QColorDialog, QPalette, QTreeWidgetItem,\
#QComboBox, QPushButton, QDoubleSpinBox, QHBoxLayout, QWidget, QSlider, QSpinBox, QLineEdit
#pyqt5 = True

if not "mobase" in sys.modules:
    import mock_mobase as mobase

class CaselessDict(dict):
    """Dictionary that enables case insensitive searching while preserving case sensitivity 
when keys are listed, ie, via keys() or items() methods. 
 
Works by storing a lowercase version of the key as the new key and stores the original key-value 
pair as the key's value (values become dictionaries)."""
 
    def __init__(self, initval={}):
        if isinstance(initval, dict):
            for key, value in initval.iteritems():
                self.__setitem__(key, value)
        elif isinstance(initval, list):
            for (key, value) in initval:
                self.__setitem__(key, value)
            
    def __contains__(self, key):
        return dict.__contains__(self, key.lower())
  
    def __getitem__(self, key):
        return dict.__getitem__(self, key.lower())['val'] 
  
    def __setitem__(self, key, value):
        return dict.__setitem__(self, key.lower(), {'key': key, 'val': value})
 
    def updateKey(self,  key):
        dict.__getitem__(self,  key.lower())['key'] = key
 
    def get(self, key, default=None):
        try:
            v = dict.__getitem__(self, key.lower())
        except KeyError:
            return default
        else:
            return v['val']
 
    def has_key(self,key):
        if self.get(key):
            return True
        else:
            return False    
 
    def items(self):
        return [(v['key'], v['val']) for v in dict.itervalues(self)]
    
    def keys(self):
        return [v['key'] for v in dict.itervalues(self)]
    
    def values(self):
        return [v['val'] for v in dict.itervalues(self)]
    
    def iteritems(self):
        for v in dict.itervalues(self):
            yield v['key'], v['val']
        
    def iterkeys(self):
        for v in dict.itervalues(self):
            yield v['key']
        
    def itervalues(self):
        for v in dict.itervalues(self):
            yield v['val']


class MainWindow(QDialog):
    saveSettings = pyqtSignal(dict)
    def __init__(self,  settings,  parent = None):
        super(MainWindow,  self).__init__(parent)
        self.__settings = settings
        from pyCfgDialog import Ui_PyCfgDialog

        self.__ui = Ui_PyCfgDialog()
        self.__ui.setupUi(self)

        self.setWindowFlags(self.windowFlags() & ~Qt.WindowContextHelpButtonHint)

        self.__updateCategories()
        self.__ui.categorySelection.currentIndexChanged[int].connect(self.__sectionChanged)
        self.__ui.advancedButton.clicked.connect(self.__advancedClicked)
        self.__ui.saveButton.clicked.connect(self.__save)
        #self.__ui.settingsTree.header().setMinimumSectionSize(200)
        if pyqt5:
            self.__ui.settingsTree.header().setSectionResizeMode(0, QHeaderView.Interactive)
            self.__ui.settingsTree.header().setSectionResizeMode(1, QHeaderView.Stretch)
        else:
            self.__ui.settingsTree.header().setResizeMode(0, QHeaderView.Interactive)
            self.__ui.settingsTree.header().setResizeMode(1, QHeaderView.Stretch)

        self.__lastSelectedCategory = ""
        self.__ui.closeButton.clicked.connect(self.close)

    def closeEvent(self,  event):
        if self.__ui.saveButton.isEnabled():
            res = QMessageBox.question(self,  "Unsaved changes", 
                            "There are unsaved changes. Do you want to save before closing the dialog?", 
                            QMessageBox.Save | QMessageBox.Discard | QMessageBox.Cancel, 
                            QMessageBox.Cancel)
            if res == QMessageBox.Save:
                self.__save()
            elif res == QMessageBox.Cancel:
                event.ignore()
        else:
            super(MainWindow,  self).__init__(event)

    def __save(self):
        self.saveSettings.emit(self.__settings)
        self.__ui.saveButton.setEnabled(False)

    def __advancedClicked(self):
        self.sender().setText("Advanced" if self.sender().isChecked() else "Basic")
        self.__updateCategories()
        newIdx = self.__ui.categorySelection.findText(self.__lastSelectedCategory)
        if newIdx != -1:
            self.__ui.categorySelection.setCurrentIndex(newIdx)

    def __valueChanged(self,  sender,  value):
        section = str(self.__ui.categorySelection.currentText())
        # this should be a qvariant according to documentation but in pyqt5 it's not?
        if type(sender.property("key")) == QtCore.QVariant:
            itemName = sender.property("key").toString()
        else:
            itemName = sender.property("key")
        setting = self.__settings[section][str(itemName)]
        setting["value"] = value
        matches = self.__ui.settingsTree.findItems(itemName,  QtCore.Qt.MatchExactly)
        item = matches[0]
        self.__updateIcon(item)

    def __updateIcon(self,  item):
        setting = self.__settings[self.__lastSelectedCategory][str(item.text(0))]
        saved = setting.get("saved",  setting["default"])
        if  saved != setting["value"]:
            item.setIcon(0, QtGui.QIcon(":/pyCfg/modified"))
            self.__ui.saveButton.setEnabled(True)
        else:
            item.setIcon(0, QtGui.QIcon(":/pyCfg/empty"))

    def __valueChangedIndex(self,  value):
        # odd problem: can't connect to the slot with text-parameter
        self.__valueChanged(self.sender(),  str(self.sender().itemText(value)))
    
    def __sliderChanged(self,  value):
        self.__valueChanged(self.sender(),  value)

    def __updateCategories(self):
        self.__ui.categorySelection.clear()
        self.__ui.categorySelection.addItem("")
        advanced = self.__ui.advancedButton.isChecked()
        for cat in self.__settings.keys():
            display = advanced
            if not display:
                for setting in self.__settings[cat].values():
                    if "basic" in setting.get("flags",  []):
                        display = True
                        continue

            if display:
                self.__ui.categorySelection.addItem(cat)

    def __rgbClicked(self):
        col = QColorDialog.getColor(self.sender().palette().color(QPalette.ButtonText))
        palette = self.sender().palette()
        palette.setColor(QPalette.ButtonText,  col)
        colStr = str(col.red()) + "," + str(col.green()) + "," + str(col.blue())
        print(colStr)
        self.__valueChanged(self.sender(),  colStr) # xxx
        self.sender().setPalette(palette)

    def __boolClicked(self):
        if self.sender().isChecked() :
            self.sender().setText("true")
            self.__valueChanged(self.sender(),  True)
            self.sender().setIcon(QtGui.QIcon(":/pyCfg/true"))
        else:
            self.sender().setText("false")
            self.__valueChanged(self.sender(),  False)
            self.sender().setIcon(QtGui.QIcon(":/pyCfg/false"))

    # qt5
    # def __editBoxChanged(self, ignore):
    def __editBoxChanged(self):
        self.__valueChanged(self.sender(),  str(self.sender().text()))

    def __floatBoxChanged(self,  value):
        self.__valueChanged(self.sender(),  value)

    def __genSelectionEntry(self,  key,  setting):
        newItem = QTreeWidgetItem(self.__ui.settingsTree,  [ key ] )
        selectionWidget = QComboBox(self.__ui.settingsTree)
        selectionWidget.setProperty("key",  key)
        for val in setting["values"]:
            selectionWidget.addItem(str(val),  val)
            if setting["value"] == val:
                selectionWidget.setCurrentIndex(selectionWidget.count() - 1)

        self.__ui.settingsTree.setItemWidget(newItem,  1,  selectionWidget)
        selectionWidget.currentIndexChanged[int].connect(self.__valueChangedIndex)
        return newItem

    def __genBooleanEntry(self,  key,  setting):
        newItem = QTreeWidgetItem(self.__ui.settingsTree,  [ key ] )
        enableBtn = QPushButton("true" if setting["value"] else "false",  self.__ui.settingsTree)
        enableBtn.setProperty("key",  key)
        enableBtn.setCheckable(True)
        enableBtn.setChecked(setting["value"])
        enableBtn.setIcon(QtGui.QIcon(":/pyCfg/true") if setting["value"] else QtGui.QIcon(":/pyCfg/false"))
        self.__ui.settingsTree.setItemWidget(newItem,  1,   enableBtn)
        enableBtn.clicked.connect(self.__boolClicked)
        return newItem

    def __genDoubleEditEntry(self,  key,  setting):
        newItem = QTreeWidgetItem(self.__ui.settingsTree,  [ key,  "" ] )
        editWidget = QDoubleSpinBox(self.__ui.settingsTree)
        if "range" in setting:
            editWidget.setRange(setting["range"]["lower"],  setting["range"]["upper"])
        else:
            editWidget.setRange(-sys.float_info.max,  sys.float_info.max)
        editWidget.setSingleStep(setting.get("step",  1.0))
        editWidget.setProperty("key",  key)
        editWidget.setValue(setting["value"])
        editWidget.valueChanged[float].connect(self.__floatBoxChanged)
        self.__ui.settingsTree.setItemWidget(newItem,  1,   editWidget)
        return newItem

    def __genSlider(self,  range,  step,  key,  value):
            newWidget = QWidget(self.__ui.settingsTree)
            layout = QHBoxLayout(newWidget)
            layout.setStretch(2,  1)
            slider = QSlider(Qt.Horizontal,  newWidget)
            spin = QSpinBox(self.__ui.settingsTree)
            slider.setProperty("key",  key)
            slider.setRange(range["lower"],  range["upper"])
            spin.setRange(range["lower"],  range["upper"])
            slider.setSingleStep(step)
            spin.setSingleStep(step)
            slider.setValue(value)
            spin.setValue(value)
            layout.addWidget(slider)
            layout.addWidget(spin)
            slider.sliderMoved.connect(spin.setValue)
            slider.valueChanged.connect(spin.setValue)
            spin.valueChanged.connect(slider.setValue)
            newWidget.setLayout(layout)
            slider.valueChanged.connect(self.__sliderChanged) # xxx
            return newWidget

    def __genIntEditEntry(self,  key,  setting):
        newItem = QTreeWidgetItem(self.__ui.settingsTree,  [ key,  "" ] )
        if "range" in setting:
            newWidget = self.__genSlider(setting["range"],  setting.get("step",  1),  str(key),  setting["value"])
        else:
            newWidget = QSpinBox(self.__ui.settingsTree)
            newWidget.setRange(-sys.maxint,  sys.maxint)
            newWidget.setSingleStep(setting.get("step",  1))
            newWidget.setProperty("key",  key)
            newWidget.setValue(setting["value"])
            newWidget.valueChanged.connect(self.__sliderChanged) # xxx
        self.__ui.settingsTree.setItemWidget(newItem,  1,   newWidget)
        return newItem

    def __genUnsignedEditEntry(self,  key,  setting):
        newItem = QTreeWidgetItem(self.__ui.settingsTree,  [ key,  "" ] )
        if "range" in setting:
            newWidget = self.__genSlider(setting["range"],  setting.get("step",  1),  key,  setting["value"])
        else:
            newWidget = QSpinBox(self.__ui.settingsTree)
            newWidget.setRange(0,   sys.maxint)
            newWidget.setSingleStep(setting.get("step",  1))
            newWidget.setProperty("key",  key)
            newWidget.setValue(setting["value"])
            newWidget.valueChanged.connect(self.__sliderChanged)
        self.__ui.settingsTree.setItemWidget(newItem,  1,   newWidget)
        return newItem

    def __genEditEntry(self,  key,  setting):
        newItem = QTreeWidgetItem(self.__ui.settingsTree,  [ key ] )
        editBox = QLineEdit(str(setting["value"]),  self.__ui.settingsTree)
        editBox.setProperty("key",  key)
        # connecting to textEdited crashes with PyQt4 for unknown reason, editingFinished does not
        editBox.editingFinished.connect(self.__editBoxChanged)
        # editBox.textEdited.connect(self.__editBoxChanged)
        self.__ui.settingsTree.setItemWidget(newItem,  1,   editBox)

        return newItem

    def __genColorEntry(self,  key,  setting):
        newItem = QTreeWidgetItem(self.__ui.settingsTree,  [ key ] )
        colorBtn = QPushButton("Color",  self.__ui.settingsTree)
        if setting["value"] == "":
            rgb = [ 0,  0,  0 ]
        else:
            rgb = [ int(col) for col in str(setting["value"]).split(",") ]
        color = QColor(rgb[0],  rgb[1],  rgb[2])
        palette = colorBtn.palette()
        palette.setColor(QPalette.ButtonText,  color)
        colorBtn.setPalette(palette)
        font = colorBtn.font()
        font.setBold(True)
        colorBtn.setFont(font)
        colorBtn.setProperty("key",  key)
        colorBtn.clicked.connect(self.__rgbClicked)
        self.__ui.settingsTree.setItemWidget(newItem,  1,   colorBtn)
        return newItem

    def __addSetting(self,  key,  setting):
        if "values" in setting:
            return self.__genSelectionEntry(key,  setting)
        else:
            keyType = key[0]
            if keyType == 'b':
                return self.__genBooleanEntry(key,  setting)
            elif keyType == 'f':
                return self.__genDoubleEditEntry(key,  setting)
            elif keyType == 'i':
                return self.__genIntEditEntry(key,  setting)
            elif keyType == 'u':
                return self.__genUnsignedEditEntry(key,  setting)
            elif keyType == 'r':
                return self.__genColorEntry(key,  setting)
            else:
                return self.__genEditEntry(key,  setting)

    def __updateTree(self):
        self.__ui.settingsTree.clear()
        if not str(self.__ui.categorySelection.currentText()) in self.__settings:
            return
        category = str(self.__ui.categorySelection.currentText())
        self.__lastSelectedCategory = category
        for settingKey in sorted(self.__settings[category].keys(),  key=lambda setKey: setKey[1:]):
            setting = self.__settings[category][settingKey]
            if not self.__ui.advancedButton.isChecked() and not "basic" in setting.get("flags",  []):
                continue
            newItem = self.__addSetting(settingKey,  setting)
            self.__updateIcon(newItem)
            if "description" in setting:
                newItem.setToolTip(0,  setting["description"])
            newItem.setToolTip(1,  "Default: " + str(setting["default"]))
            self.__ui.settingsTree.addTopLevelItem(newItem)

        self.__ui.settingsTree.resizeColumnToContents(0)

    def __sectionChanged(self,  sectionIdx):
        self.__updateTree()



class MagicFile(file):
    def __init__(self, filename, mode):
        super(MagicFile, self).__init__(filename, mode)

    def readline(self, size=-1):
        temp = super(MagicFile, self).readline(size)
        if len(temp) == 0:
            return temp

        esidx = temp.find('=')
        # comment
        if temp[0] == ';' or temp[0] == '#' or temp[0] == '[':
            return temp
        # no equals sign
        if esidx == -1:
            return "  " + temp
        else:
          return temp


class IniEdit(mobase.IPluginTool):
    def init(self, organizer):
        import pyCfgResource_rc

        self.__organizer = organizer
        try:
                f = open(organizer.pluginDataPath() + "/settings.json", "r")
        except IOError:
                return False
        self.__settings = json.loads(f.read())
        f.close()
        return True

    def name(self):
        return "Configurator"

    def author(self):
        return "Tannin"

    def description(self):
        return "Plugin to allow easier customization of game settings"

    def version(self):
        return mobase.VersionInfo(1, 0, 0, mobase.ReleaseType.beta)

    def isActive(self):
        return True

    def settings(self):
        return []

    def displayName(self):
        return "Configurator"

    def tooltip(self):
        return "Modify game Configuration"

    def icon(self):
        return QtGui.QIcon(":/pyCfg/pycfgicon")

    def setParentWidget(self, widget):
        self.__parentWidget = widget

    def __iniFiles(self):
        gameType = self.__organizer.gameInfo().type()
        if str(gameType) == "oblivion":
            return [ "oblivion.ini",  "oblivionprefs.ini" ]
        elif str(gameType) == "fallout3" or str(gameType) == "falloutnv":
            return [ "fallout.ini",  "falloutprefs.ini" ]
        elif str(gameType) == "skyrim":
            return [ "skyrim.ini",  "skyrimprefs.ini" ]
        else:
            return []

    def __filteredSettings(self):
        newSettings = CaselessDict()
        gameName = str(self.__organizer.gameInfo().type())
        
        iniFiles = self.__iniFiles()
        
        for sectionKey in self.__settings.keys():
            section = self.__settings[sectionKey]
            filteredSection = CaselessDict()
            for key,  setting in section.iteritems():
                setting["value"] = setting["default"]   
                setting["file"] = iniFiles[1] if "prefs" in setting.get("flags",  []) else iniFiles[0]
                if "hidden" in setting.get("flags",  []):
                    # set to hidden
                    continue
                if "games" in setting and not gameName in setting["games"]:
                    # not for this game
                    continue
                filteredSection[str(key)] = setting
            if len(filteredSection) > 0:
                newSettings[sectionKey] = filteredSection
        return newSettings

    def updateSettings(self,  settings,  file):
        parser = ConfigParser.SafeConfigParser(allow_no_value=True)
        parser.optionxform = str
        file = MagicFile(self.__organizer.profilePath() + "/" + file, 'r')
        parser.readfp(file)
        for section in parser.sections():
            if not section in settings:
                settings[section] = CaselessDict()
            else:
                settings.updateKey(section)

            for setting in parser.items(section, True):
                newData = settings[section].get(setting[0],  {})
                value = setting[1].split('//')[0].strip()
                if setting[0][0] == 'b':
                    value = True if value == "1" else False
                elif setting[0][0] == 'i' or setting[0][0] == 'u':
                    value = int(value)
                elif setting[0][0] == 'f':
                    value = float(value)
                newData["value"] = value
                newData["saved"] = value
                newData["file"] = file
                if not "default" in newData:
                    newData["default"] = value
                settings[section][str(setting[0])] = newData

    def __save(self,  settings):
        iniFiles = {}
        for fileName in self.__iniFiles():
            parser = ConfigParser.SafeConfigParser()
            parser.optionxform = str
            parser.read(self.__organizer.profilePath() + "/" + fileName)
            iniFiles[fileName]  = parser

        count = 0
        for sectionkey, section in settings.iteritems():
            count += 1
            for settingkey,  setting in section.iteritems():
                if setting["value"] != setting.get("saved",  setting["default"]):
                    try:
                        iniFiles[setting["file"]].add_section(sectionkey)
                    except ConfigParser.DuplicateSectionError:
                        pass

                    if type(setting["value"]) == bool:
                        iniFiles[setting["file"]].set(sectionkey,  settingkey,  '1' if setting["value"] else '0')
                    else:
                        iniFiles[setting["file"]].set(sectionkey,  settingkey,  str(setting["value"]))
                    setting["saved"] = setting["value"]

        for fileName,  data in iniFiles.iteritems():
            out = open(self.__organizer.profilePath() + "/" + fileName,  'w')
            data.write(out)
            out.close()

    def display(self):
        settings = self.__filteredSettings()
        for iniFile in self.__iniFiles():
            self.updateSettings(settings, iniFile)

        win = MainWindow(settings)
        win.saveSettings.connect(self.__save)
        win.exec_()

def createPlugin():
        return IniEdit()
