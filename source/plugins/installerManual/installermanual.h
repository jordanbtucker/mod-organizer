/*
Copyright (C) 2012 Sebastian Herbord. All rights reserved.

This file is part of Mod Organizer.

Mod Organizer is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Mod Organizer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Mod Organizer.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INSTALLERMANUAL_H
#define INSTALLERMANUAL_H


#include <iplugininstallersimple.h>


class InstallerManual : public MOBase::IPluginInstallerSimple
{
  Q_OBJECT
  Q_INTERFACES(MOBase::IPlugin MOBase::IPluginInstaller MOBase::IPluginInstallerSimple)
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  Q_PLUGIN_METADATA(IID "org.tannin.InstallerManual" FILE "installermanual.json")
#endif

public:

  InstallerManual();

  virtual bool init(MOBase::IOrganizer *moInfo);
  virtual QString name() const;
  virtual QString author() const;
  virtual QString description() const;
  virtual MOBase::VersionInfo version() const;
  virtual bool isActive() const;
  virtual QList<MOBase::PluginSetting> settings() const;

  virtual unsigned int priority() const;
  virtual bool isManualInstaller() const;

  virtual bool isArchiveSupported(const MOBase::DirectoryTree &tree) const;
  virtual EInstallResult install(MOBase::GuessedValue<QString> &modName, MOBase::DirectoryTree &tree,
                                 QString &version, int &modID);

private:

  bool isSimpleArchiveTopLayer(const MOBase::DirectoryTree::Node *node) const;
  const MOBase::DirectoryTree::Node *getSimpleArchiveBase(const MOBase::DirectoryTree &dataTree) const;

private slots:

  /**
   * @brief opens a file from the archive in the (system-)default editor/viewer
   * @param fileName relative name of the file to open
   */
  void openFile(const QString &fileName);

private:

  const MOBase::IOrganizer *m_MOInfo;

};


#endif // INSTALLERMANUAL_H
