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

#include "installermanual.h"
#include "installdialog.h"

#include <utility.h>
#include <iinstallationmanager.h>

#include <QtPlugin>
#include <QDialog>

#include <Shellapi.h>


using namespace MOBase;


InstallerManual::InstallerManual()
  : m_MOInfo(nullptr)
{
}

bool InstallerManual::init(IOrganizer *moInfo)
{
  m_MOInfo = moInfo;
  return true;
}

QString InstallerManual::name() const
{
  return "Manual Installer";
}

QString InstallerManual::author() const
{
  return "Tannin";
}

QString InstallerManual::description() const
{
  return tr("Fallback installer for mods that can be extracted but can't be handled by another installer");
}

VersionInfo InstallerManual::version() const
{
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
}

bool InstallerManual::isActive() const
{
  return true;
}

QList<PluginSetting> InstallerManual::settings() const
{
  return QList<PluginSetting>();
}

unsigned int InstallerManual::priority() const
{
  return 0;
}


bool InstallerManual::isManualInstaller() const
{
  return true;
}


bool InstallerManual::isArchiveSupported(const DirectoryTree&) const
{
  return true;
}


void InstallerManual::openFile(const QString &fileName)
{
  QString tempName = manager()->extractFile(fileName);

  SHELLEXECUTEINFOW execInfo;
  memset(&execInfo, 0, sizeof(SHELLEXECUTEINFOW));
  execInfo.cbSize = sizeof(SHELLEXECUTEINFOW);
  execInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
  execInfo.lpVerb = L"open";
  std::wstring fileNameW = ToWString(tempName);
  execInfo.lpFile = fileNameW.c_str();
  execInfo.nShow = SW_SHOWNORMAL;
  if (!::ShellExecuteExW(&execInfo)) {
    qCritical("failed to spawn %s: %d", tempName.toUtf8().constData(), ::GetLastError());
  }
}


IPluginInstaller::EInstallResult InstallerManual::install(GuessedValue<QString> &modName, DirectoryTree &tree,
                                                          QString&, int&)
{
  qDebug("offering installation dialog");
  InstallDialog dialog(&tree, modName, parentWidget());
  connect(&dialog, SIGNAL(openFile(QString)), this, SLOT(openFile(QString)));
  if (dialog.exec() == QDialog::Accepted) {
    modName.update(dialog.getModName(), GUESS_USER);

    // TODO probably more complicated than necessary
    DirectoryTree *newTree = dialog.getModifiedTree();
    tree = *newTree;
    delete newTree;
    return IPluginInstaller::RESULT_SUCCESS;
  } else {
    return IPluginInstaller::RESULT_CANCELED;
  }
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(installerManual, InstallerManual)
#endif

