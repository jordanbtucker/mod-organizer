/*
Copyright (C) 2013 Sebastian Herbord. All rights reserved.

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


#ifndef PAGETESALLIANCE_H
#define PAGETESALLIANCE_H

#include <ipluginmodpage.h>
#include <modrepositoryfileinfo.h>

class PageTESAlliance : public MOBase::IPluginModPage
{

  Q_OBJECT
  Q_INTERFACES(MOBase::IPlugin MOBase::IPluginModPage)
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  Q_PLUGIN_METADATA(IID "org.tannin.PageTESAlliance" FILE "pageTesAlliance.json")
#endif

public:
  PageTESAlliance();

  // IPlugin interface
public:
  virtual bool init(MOBase::IOrganizer *moInfo);
  virtual QString name() const;
  virtual QString author() const;
  virtual QString description() const;
  virtual MOBase::VersionInfo version() const;
  virtual bool isActive() const;
  virtual QList<MOBase::PluginSetting> settings() const;

  // IPluginModPage interface
public:
  virtual QString displayName() const;
  virtual QIcon icon() const;
  virtual QUrl pageURL() const;
  virtual bool handlesDownload(const QUrl &pageURL, const QUrl &downloadURL, MOBase::ModRepositoryFileInfo &fileInfo) const;
  virtual bool useIntegratedBrowser() const;

private:
  QString convertModName(const QString &inputName) const;

};

#endif // PAGETESALLIANCE_H
