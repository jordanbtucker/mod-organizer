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

#include "directoryrefresher.h"
#include "utility.h"
#include "report.h"
#include "modinfo.h"
#include <gameinfo.h>
#include <QDir>
#include <QString>


using namespace MOBase;
using namespace MOShared;


DirectoryRefresher::DirectoryRefresher()
  : m_DirectoryStructure(NULL)
{
}

DirectoryRefresher::~DirectoryRefresher()
{
  delete m_DirectoryStructure;
}

DirectoryEntry *DirectoryRefresher::getDirectoryStructure()
{
  QMutexLocker locker(&m_RefreshLock);
  DirectoryEntry *result = m_DirectoryStructure;
  m_DirectoryStructure = NULL;
  return result;
}

void DirectoryRefresher::setMods(const std::vector<std::tuple<QString, QString, int> > &mods
                                 , const std::set<QString> &managedArchives)
{
  QMutexLocker locker(&m_RefreshLock);

  m_Mods.clear();
  for (auto mod = mods.begin(); mod != mods.end(); ++mod) {
    QString name = std::get<0>(*mod);
    ModInfo::Ptr info = ModInfo::getByIndex(ModInfo::getIndex(name));
    m_Mods.push_back(EntryInfo(name, std::get<1>(*mod), info->stealFiles(), info->archives(), std::get<2>(*mod)));
  }

  m_EnabledArchives = managedArchives;
}


void DirectoryRefresher::cleanStructure(DirectoryEntry *structure)
{
  static wchar_t *files[] = { L"meta.ini", L"readme.txt" };
  for (int i = 0; i < sizeof(files) / sizeof(wchar_t*); ++i) {
    structure->removeFile(files[i]);
  }

  static wchar_t *dirs[] = { L"fomod" };
  for (int i = 0; i < sizeof(dirs) / sizeof(wchar_t*); ++i) {
    structure->removeDir(std::wstring(dirs[i]));
  }
}

void DirectoryRefresher::addModToStructure(DirectoryEntry *directoryStructure
                                           , const QString &modName
                                           , int priority
                                           , const QString &directory
                                           , const QStringList &stealFiles
                                           , const QStringList &archives)
{
  std::wstring directoryW = ToWString(QDir::toNativeSeparators(directory));
  std::wstring modNameW = ToWString(modName);


  if (stealFiles.length() > 0) {
    // instead of adding all the files of the target directory, we just change the root of the specified
    // files to this mod
    directoryStructure->createOrigin(modNameW, directoryW, priority);
    foreach (const QString &filename, stealFiles) {
      QFileInfo fileInfo(filename);
      FileEntry::Ptr file = directoryStructure->findFile(ToWString(fileInfo.fileName()));
      if (file.get() != NULL) {
        if (file->getOrigin() == 0) {
          // replace data as the origin on this bsa
          file->removeOrigin(0);
          file->addOrigin(directoryStructure->getOriginByName(modNameW).getID(),
                          file->getFileTime(), L"");
        }
      }
    }
  } else {
    directoryStructure->addFromOrigin(modNameW, directoryW, priority);
  }
/*  QDir dir(directory);
  QFileInfoList bsaFiles = dir.entryInfoList(QStringList("*.bsa"), QDir::Files);
  foreach (QFileInfo file, bsaFiles) {
    if (m_EnabledArchives.find(file.fileName()) != m_EnabledArchives.end()) {
      directoryStructure->addFromBSA(ToWString(modName), directoryW,
                                     ToWString(QDir::toNativeSeparators(file.absoluteFilePath())), priority);
    }
  }*/

  foreach (const QString &archive, archives) {
    QFileInfo fileInfo(archive);
    if (m_EnabledArchives.find(fileInfo.fileName()) != m_EnabledArchives.end()) {
      directoryStructure->addFromBSA(modNameW, directoryW,
                                     ToWString(QDir::toNativeSeparators(fileInfo.absoluteFilePath())), priority);
    }
  }
}

void DirectoryRefresher::refresh()
{
  QMutexLocker locker(&m_RefreshLock);

  delete m_DirectoryStructure;

  m_DirectoryStructure = new DirectoryEntry(L"data", NULL, 0);

  std::wstring dataDirectory = GameInfo::instance().getGameDirectory() + L"\\data";
  m_DirectoryStructure->addFromOrigin(L"data", dataDirectory, 0);

  // TODO what was the point of having the priority in this tuple? the list is already sorted by priority
  auto iter = m_Mods.begin();

  //TODO i is the priority here, where higher = more important. the input vector is also sorted by priority but inverted!
  for (int i = 1; iter != m_Mods.end(); ++iter, ++i) {
qDebug("%s - %d", qPrintable(iter->modName), i);
    try {
      addModToStructure(m_DirectoryStructure, iter->modName, i, iter->absolutePath, iter->stealFiles, iter->archives);
    } catch (const std::exception &e) {
      emit error(tr("failed to read bsa: %1").arg(e.what()));
    }
    emit progress((i * 100) / m_Mods.size() + 1);
  }

  emit progress(100);

  cleanStructure(m_DirectoryStructure);

  emit refreshed();
}
