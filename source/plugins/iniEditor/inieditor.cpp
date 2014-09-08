/*
Copyright (C) 2012 Sebastian Herbord. All rights reserved.

This file is part of ini editor plugin for MO

Ini editor plugin is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Ini editor plugin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Ini editor plugin.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "inieditor.h"
#include "textviewer.h"
#include <utility.h>
#include <QtPlugin>
#include <QMessageBox>
#include <QDir>
#include <QApplication>
#include <imoinfo.h>


using namespace MOBase;


IniEditor::IniEditor()
  : m_MOInfo(NULL)
{
}

bool IniEditor::init(IOrganizer *moInfo)
{
  m_MOInfo = moInfo;
  return true;
}

QString IniEditor::name() const
{
  return "INI Editor";
}

QString IniEditor::author() const
{
  return "Tannin";
}

QString IniEditor::description() const
{
  return tr("Very basic editor for ini files");
}

VersionInfo IniEditor::version() const
{
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
}

bool IniEditor::isActive() const
{
  return true;
}

QList<PluginSetting> IniEditor::settings() const
{
  QList<PluginSetting> result;
  result.push_back(PluginSetting("external", "Use an external editor to open the files", QVariant(false)));
  result.push_back(PluginSetting("associated", "When using an external editor, use the application associated with \"ini\" files. "
                                 "If false, uses the \"edit\" command which usually invokes regular notepad.", QVariant(true)));
  return result;
}

QString IniEditor::displayName() const
{
  return tr("INI Editor");
}

QString IniEditor::tooltip() const
{
  return tr("Edit the ini file of the current profile");
}

QIcon IniEditor::icon() const
{
  return QIcon(":/inieditor/icon_document");
}

void IniEditor::display() const
{
  if (m_MOInfo == NULL) {
    throw MyException(tr("plugin not initialized"));
  }

  std::vector<QString> iniFiles = getIniFiles();
  if (m_MOInfo->pluginSetting(name(), "external").toBool()) {
    for (std::vector<QString>::iterator iter = iniFiles.begin(); iter != iniFiles.end(); ++iter) {
      QString fileName = QString("%1/profiles/%2/%3").arg(QApplication::applicationDirPath())
                                                     .arg(m_MOInfo->profileName())
                                                     .arg(*iter);
      ::ShellExecuteW(NULL,m_MOInfo->pluginSetting(name(), "associated").toBool() ? L"open" : L"edit",
                      ToWString(fileName).c_str(), NULL, NULL, SW_SHOWNORMAL);
    }
  } else {
    TextViewer *viewer = new TextViewer("Ini Files", parentWidget());
    viewer->setDescription(tr("Ini files are local to the currently selected profile."));
    for (std::vector<QString>::iterator iter = iniFiles.begin(); iter != iniFiles.end(); ++iter) {
      QString fileName = QString("%1/profiles/%2/%3").arg(QApplication::applicationDirPath())
                                                     .arg(m_MOInfo->profileName())
                                                     .arg(*iter);
      if (QFileInfo(fileName).exists()) {
        viewer->addFile(fileName, true);
      }
    }
    viewer->show();
  }
}


std::vector<QString> IniEditor::getIniFiles() const
{
  std::vector<QString> iniFiles;
  IGameInfo::Type type = m_MOInfo->gameInfo().type();
  switch (type) {
    case IGameInfo::TYPE_OBLIVION: {
      iniFiles.push_back("oblivion.ini");
      iniFiles.push_back("oblivionprefs.ini");
    } break;
    case IGameInfo::TYPE_FALLOUT3:
    case IGameInfo::TYPE_FALLOUTNV: {
      iniFiles.push_back("fallout.ini");
      iniFiles.push_back("falloutprefs.ini");
    } break;
    case IGameInfo::TYPE_SKYRIM: {
      iniFiles.push_back("skyrim.ini");
      iniFiles.push_back("skyrimprefs.ini");
    } break;
  }

  return iniFiles;
}


#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(iniEditor, IniEditor)
#endif
