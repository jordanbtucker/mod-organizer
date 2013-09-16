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

#include "profile.h"
#include "report.h"
#include "gameinfo.h"
#include "windows_error.h"
#include "dummybsa.h"
#include "modinfo.h"
#include <utility.h>
#include <util.h>
#include <error_report.h>
#include <appconfig.h>
#include <QMessageBox>
#include <QApplication>
#include <QSettings>
#include <QTemporaryFile>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include <stdexcept>

using namespace MOBase;
using namespace MOShared;

Profile::Profile()
  : m_SaveTimer(NULL)
{
  initTimer();
}

Profile::Profile(const QString &name, bool useDefaultSettings)
  : m_SaveTimer(NULL)
{
  initTimer();
  QString profilesDir = QDir::fromNativeSeparators(ToQString(GameInfo::instance().getProfilesDir()));
  QDir profileBase(profilesDir);

  QString fixedName = name;
  if (!fixDirectoryName(fixedName)) {
    throw MyException(tr("invalid profile name %1").arg(name));
  }

  if (!profileBase.exists() || !profileBase.mkdir(fixedName)) {
    throw MyException(tr("failed to create %1").arg(fixedName).toUtf8().constData());
  }
  QString fullPath = profilesDir + "/" + fixedName;
  m_Directory = QDir(fullPath);
  QFile modList(m_Directory.filePath("modlist.txt"));
  if (!modList.open(QIODevice::ReadWrite)) {
    profileBase.rmdir(fixedName);
    throw std::runtime_error(QObject::tr("failed to create %1").arg(m_Directory.filePath("modlist.txt")).toUtf8().constData());
  }
  modList.close();

  try {
    GameInfo::instance().createProfile(ToWString(fullPath), useDefaultSettings);
  } catch (...) {
    // clean up in case of an error
    shellDelete(QStringList(profileBase.absoluteFilePath(fixedName)));
    throw;
  }
  refreshModStatus();
}


Profile::Profile(const QDir& directory)
  : m_Directory(directory), m_SaveTimer(NULL)
{
  initTimer();
  if (!QFile::exists(m_Directory.filePath("modlist.txt"))) {
    qWarning("missing modlist.txt in %s", qPrintable(directory.path()));
  }

  GameInfo::instance().repairProfile(ToWString(m_Directory.absolutePath()));

  if (!QFile::exists(getIniFileName())) {
    reportError(QObject::tr("\"%1\" is missing").arg(getIniFileName()));
  }
  refreshModStatus();
}


Profile::Profile(const Profile& reference)
  : m_Directory(reference.m_Directory), m_SaveTimer(NULL)
{
  initTimer();
  refreshModStatus();
}


Profile::~Profile()
{
  writeModlistNow();
}


void Profile::initTimer()
{
  m_SaveTimer = new QTimer(this);
  m_SaveTimer->setSingleShot(true);
  connect(m_SaveTimer, SIGNAL(timeout()), this, SLOT(writeModlistNow()));
}


bool Profile::exists() const
{
  return m_Directory.exists();
}


void Profile::writeModlist() const
{
  if (!m_SaveTimer->isActive()) {
    m_SaveTimer->start(2000);
  }
}


void Profile::cancelWriteModlist() const
{
  m_SaveTimer->stop();
}


void Profile::writeModlistNow(bool onlyOnTimer) const
{
  if (onlyOnTimer && !m_SaveTimer->isActive()) return;

  m_SaveTimer->stop();
  if (!m_Directory.exists()) return;
#pragma message("right now, this is doing unnecessary saves. Need a flag that says that mod priority, enabled-state or name of a mod has changed")

  try {
    QTemporaryFile file;

    if (!file.open()) {
      reportError(tr("failed to open temporary file"));
      return;
    }

    file.resize(0);
    file.write(QString("# This file was automatically generated by Mod Organizer.\r\n").toUtf8());
    if (m_ModStatus.empty()) {
      return;
    }

    for (int i = m_ModStatus.size() - 1; i >= 0; --i) {
      // the priority order was inverted on load so it has to be inverted again
      unsigned int index = m_ModIndexByPriority[i];
      if (index != UINT_MAX) {
        ModInfo::Ptr modInfo = ModInfo::getByIndex(index);
        if (modInfo->getFixedPriority() == INT_MIN) {
          if (m_ModStatus[index].m_Enabled) {
            file.write("+");
          } else {
            file.write("-");
          }
          file.write(modInfo->name().toUtf8());
          file.write("\r\n");
        }
      }
    }

    file.close();


    QString fileName = getModlistFileName();

    if (QFile::exists(fileName)) {
      shellDelete(QStringList(fileName));
    }

    if (!file.copy(fileName)) {
      reportError(tr("failed to open \"%1\" for writing").arg(fileName));
      return;
    }

    qDebug("%s saved", QDir::toNativeSeparators(fileName).toUtf8().constData());
  } catch (const std::exception &e) {
    reportError(tr("failed to write mod list: %1").arg(e.what()));
    return;
  }
}


void Profile::createTweakedIniFile()
{
  QString tweakedIni = m_Directory.absoluteFilePath("initweaks.ini");

  if (QFile::exists(tweakedIni) && !shellDelete(QStringList(tweakedIni))) {
    reportError(tr("failed to update tweaked ini file, wrong settings may be used: %1").arg(windowsErrorString(::GetLastError())));
    return;
  }

  for (unsigned int i = 0; i < m_ModStatus.size(); ++i) {
    if (m_ModStatus[i].m_Enabled) {
      ModInfo::Ptr modInfo = ModInfo::getByIndex(i);
      mergeTweaks(modInfo, tweakedIni);
    }
  }


  bool error = false;
  if (!::WritePrivateProfileStringW(L"Archive", L"bInvalidateOlderFiles", L"1", ToWString(tweakedIni).c_str())) {
    error = true;
  }

  if (localSavesEnabled()) {
    if (!::WritePrivateProfileStringW(L"General", L"bUseMyGamesDirectory", L"1", ToWString(tweakedIni).c_str())) {
      error = true;
    }

    if (!::WritePrivateProfileStringW(L"General", L"SLocalSavePath",
                                       AppConfig::localSavePlaceholder(),
                                       ToWString(tweakedIni).c_str())) {
      error = true;
    }
  }

  if (error) {
    reportError(tr("failed to create tweaked ini: %1").arg(getCurrentErrorStringA().c_str()));
  }
}


void Profile::refreshModStatus()
{
  QFile file(getModlistFileName());
  if (!file.exists()) {
    throw MyException(QObject::tr("failed to find \"%1\"").arg(getModlistFileName()));
  }

  bool modStatusModified = false;
  m_ModStatus.clear();
  m_ModStatus.resize(ModInfo::getNumMods());

  std::set<QString> namesRead;

  // load mods from file and update enabled state and priority for them
  file.open(QIODevice::ReadOnly);
  int index = 0;
  while (!file.atEnd()) {
    QByteArray line = file.readLine();
    bool enabled = true;
    QString modName;
    if (line.length() == 0) {
      // empty line
      continue;
    } else if (line.at(0) == '#') {
      // comment line
      continue;
    } else if (line.at(0) == '-') {
      enabled = false;
      modName = QString::fromUtf8(line.mid(1).trimmed().constData());
    } else if (line.at(0) == '+') {
      modName = QString::fromUtf8(line.mid(1).trimmed().constData());
    } else {
      modName = QString::fromUtf8(line.trimmed().constData());
    }
    if (modName.size() > 0) {
      if (namesRead.find(modName) != namesRead.end()) {
        continue;
      } else {
        namesRead.insert(modName);
      }
      unsigned int modindex = ModInfo::getIndex(modName);
      if (modindex != UINT_MAX) {
        ModInfo::Ptr info = ModInfo::getByIndex(modindex);
        if ((modindex < m_ModStatus.size()) && (info->getFixedPriority() == INT_MIN)) {
          m_ModStatus[modindex].m_Enabled = enabled;
          if (m_ModStatus[modindex].m_Priority == -1) {
            if (static_cast<size_t>(index) >= m_ModStatus.size()) {
              throw MyException(tr("invalid index %1").arg(index));
            }
            m_ModStatus[modindex].m_Priority = index++;
          }
        } else {
          qDebug("mod \"%s\" (profile \"%s\") not found",
                 modName.toUtf8().constData(), m_Directory.path().toUtf8().constData());
          // need to rewrite the modlist to fix this
          modStatusModified = true;
        }
      }
    } else {
      // line was empty after trimming
    }
  }

  int numKnownMods = index;

  // invert priority order to match that of the pluginlist. Also
  // give priorities to mods not referenced in the profile
  for (size_t i = 0; i < m_ModStatus.size(); ++i) {
    ModInfo::Ptr modInfo = ModInfo::getByIndex(i);
    if (!modInfo->canBeEnabled()) {
      continue;
    }
    if (m_ModStatus[i].m_Priority != -1) {
      m_ModStatus[i].m_Priority = numKnownMods - m_ModStatus[i].m_Priority - 1;
    } else {
      if (static_cast<size_t>(index) >= m_ModStatus.size()) {
        throw MyException(tr("invalid index %1").arg(index));
      }
      m_ModStatus[i].m_Priority = index++;
      // also, mark the mod-list as changed
      modStatusModified = true;
    }
  }

  file.close();
  updateIndices();
  if (modStatusModified) {
    writeModlist();
  }
}


void Profile::dumpModStatus() const
{
  for (unsigned int i = 0; i < m_ModStatus.size(); ++i) {
    ModInfo::Ptr info = ModInfo::getByIndex(i);
    qWarning("%d: %s - %d (%s)", i, info->name().toUtf8().constData(), m_ModStatus[i].m_Priority,
             m_ModStatus[i].m_Enabled ? "enabled" : "disabled");
  }
}


void Profile::updateIndices()
{
  m_NumRegularMods = 0;
  m_ModIndexByPriority.clear();
  m_ModIndexByPriority.resize(m_ModStatus.size(), UINT_MAX);
  for (unsigned int i = 0; i < m_ModStatus.size(); ++i) {
    int priority = m_ModStatus[i].m_Priority;
    if (priority < 0) {
      // don't assign this to mapping at all, it's probably the overwrite mod
      continue;
    } else if (priority >= static_cast<int>(m_ModIndexByPriority.size())) {
      qCritical("invalid priority %d for mod", priority);
      continue;
    } else {
      ++m_NumRegularMods;
      m_ModIndexByPriority.at(priority) = i;
    }
  }
}


std::vector<std::tuple<QString, QString, int> > Profile::getActiveMods()
{
  std::vector<std::tuple<QString, QString, int> > result;
  for (std::vector<unsigned int>::const_iterator iter = m_ModIndexByPriority.begin();
       iter != m_ModIndexByPriority.end(); ++iter) {
    if ((*iter != UINT_MAX) && m_ModStatus[*iter].m_Enabled) {
      ModInfo::Ptr modInfo = ModInfo::getByIndex(*iter);
      result.push_back(std::make_tuple(modInfo->name(), modInfo->absolutePath(), m_ModStatus[*iter].m_Priority));
    }
  }

  unsigned int overwriteIndex = ModInfo::findMod([](ModInfo::Ptr mod) -> bool {
    std::vector<ModInfo::EFlag> flags = mod->getFlags();
    return std::find(flags.begin(), flags.end(), ModInfo::FLAG_OVERWRITE) != flags.end(); });

  if (overwriteIndex != UINT_MAX) {
    ModInfo::Ptr overwriteInfo = ModInfo::getByIndex(overwriteIndex);
    result.push_back(std::make_tuple(overwriteInfo->name(), overwriteInfo->absolutePath(), UINT_MAX));
  } else {
    reportError(tr("Overwrite directory couldn't be parsed"));
  }
  return result;
}


unsigned int Profile::modIndexByPriority(unsigned int priority) const
{
  if (priority >= m_ModStatus.size()) {
    throw MyException(tr("invalid priority %1").arg(priority));
  }

  return m_ModIndexByPriority[priority];
}


void Profile::setModEnabled(unsigned int index, bool enabled)
{
  if (index >= m_ModStatus.size()) {
    throw MyException(tr("invalid index %1").arg(index));
  }

  if (m_ModStatus[index].m_Overwrite) {
    // overwrite is always enabled
    return;
  }

  if (enabled != m_ModStatus[index].m_Enabled) {
    m_ModStatus[index].m_Enabled = enabled;
    emit modStatusChanged(index);
  }
}


bool Profile::modEnabled(unsigned int index) const
{
  if (index >= m_ModStatus.size()) {
    throw MyException(tr("invalid index %1").arg(index));
  }

  return m_ModStatus[index].m_Enabled;
}


int Profile::getModPriority(unsigned int index) const
{
  if (index >= m_ModStatus.size()) {
    throw MyException(tr("invalid index %1").arg(index));
  }

  return m_ModStatus[index].m_Priority;
}


void Profile::setModPriority(unsigned int index, int &newPriority)
{
  if (m_ModStatus.at(index).m_Overwrite) {
    // can't change priority of the overwrite
    return;
  }

  int newPriorityTemp = (std::max)(0, (std::min<int>)(m_ModStatus.size() - 1, newPriority));

  // don't try to place below overwrite
  while ((m_ModIndexByPriority.at(newPriorityTemp) >= m_ModStatus.size()) ||
         m_ModStatus.at(m_ModIndexByPriority.at(newPriorityTemp)).m_Overwrite) {
    --newPriorityTemp;
  }

  int oldPriority = m_ModStatus.at(index).m_Priority;
  if (newPriorityTemp > oldPriority) {
    // priority is higher than the old, so the gap we left is in lower priorities
    for (int i = oldPriority + 1; i <= newPriorityTemp; ++i) {
      --m_ModStatus.at(m_ModIndexByPriority.at(i)).m_Priority;
    }
  } else {
    for (int i = newPriorityTemp; i < oldPriority; ++i) {
      ++m_ModStatus.at(m_ModIndexByPriority.at(i)).m_Priority;
    }
    ++newPriority;
  }

  m_ModStatus.at(index).m_Priority = newPriorityTemp;

  updateIndices();
  writeModlist();
}


Profile Profile::createFrom(const QString &name, const Profile &reference)
{
  QString profileDirectory = QDir::fromNativeSeparators(ToQString(GameInfo::instance().getProfilesDir())).append("/").append(name);
  reference.copyFilesTo(profileDirectory);
  return Profile(QDir(profileDirectory));
}


Profile *Profile::createPtrFrom(const QString &name, const Profile &reference)
{
  QString profileDirectory = QDir::fromNativeSeparators(ToQString(GameInfo::instance().getProfilesDir())).append("/").append(name);
  reference.copyFilesTo(profileDirectory);
  return new Profile(QDir(profileDirectory));
}


void Profile::copyFilesTo(QString &target) const
{
  copyDir(m_Directory.absolutePath(), target, false);
}


std::vector<std::wstring> Profile::splitDZString(const wchar_t *buffer) const
{
  std::vector<std::wstring> result;
  const wchar_t *pos = buffer;
  size_t length = wcslen(pos);
  while (length != 0U) {
    result.push_back(pos);
    pos += length + 1;
    length = wcslen(pos);
  }
  return result;
}


void Profile::mergeTweak(const QString &tweakName, const QString &tweakedIni) const
{
  static const int bufferSize = 32768;

  std::wstring tweakNameW  = ToWString(tweakName);
  std::wstring tweakedIniW = ToWString(tweakedIni);
  QScopedArrayPointer<wchar_t> buffer(new wchar_t[bufferSize]);

  // retrieve a list of sections
  DWORD size = ::GetPrivateProfileSectionNamesW(
        buffer.data(), bufferSize, tweakNameW.c_str());

  if (size == bufferSize - 2) {
    // unfortunately there is no good way to find the required size
    // of the buffer
    throw MyException(QString("Buffer too small. Please report this as a bug. "
                        "For now you might want to split up %1").arg(tweakName));
  }

  std::vector<std::wstring> sections = splitDZString(buffer.data());

  // now iterate over all sections and retrieve a list of keys in each
  for (std::vector<std::wstring>::iterator iter = sections.begin();
       iter != sections.end(); ++iter) {
    // retrieve the names of all keys
    size = ::GetPrivateProfileStringW(iter->c_str(), NULL, NULL, buffer.data(),
                                      bufferSize, tweakNameW.c_str());
    if (size == bufferSize - 2) {
      throw MyException(QString("Buffer too small. Please report this as a bug. "
                          "For now you might want to split up %1").arg(tweakName));
    }

    std::vector<std::wstring> keys = splitDZString(buffer.data());

    for (std::vector<std::wstring>::iterator keyIter = keys.begin();
         keyIter != keys.end(); ++keyIter) {
      //TODO this treats everything as strings but how could I differentiate the type?
      ::GetPrivateProfileStringW(iter->c_str(), keyIter->c_str(),
                                 NULL, buffer.data(), bufferSize, ToWString(tweakName).c_str());
      ::WritePrivateProfileStringW(iter->c_str(), keyIter->c_str(),
                                   buffer.data(), tweakedIniW.c_str());
    }
  }
}

void Profile::mergeTweaks(ModInfo::Ptr modInfo, const QString &tweakedIni) const
{
  std::vector<QString> iniTweaks = modInfo->getIniTweaks();
  for (std::vector<QString>::iterator iter = iniTweaks.begin();
       iter != iniTweaks.end(); ++iter) {
    mergeTweak(*iter, tweakedIni);
  }
}


bool Profile::invalidationActive(bool *supported) const
{
  if (GameInfo::instance().requiresBSAInvalidation()) {
    *supported = true;
    wchar_t buffer[1024];
    std::wstring iniFileName = ToWString(QDir::toNativeSeparators(getIniFileName()));
    // epic ms fail: GetPrivateProfileString uses errno (for whatever reason) to signal a fail since the return value
    // has a different meaning (number of bytes copied). HOWEVER, it will not set errno to 0 if NO error occured
    errno = 0;
    if (::GetPrivateProfileStringW(L"Archive", GameInfo::instance().archiveListKey().c_str(),
                                   L"", buffer, 1024, iniFileName.c_str()) == 0) {
      if (errno != 0x02) {
        if (supported != NULL) {
          *supported = false;
        }
        return false;
      } else {
        QString errorMessage = tr("failed to parse ini file (%1)").arg(ToQString(iniFileName));
        throw windows_error(errorMessage.toUtf8().constData());
      }
    }
    QStringList archives = ToQString(buffer).split(',');

    for (int i = 0; i < archives.count(); ++i) {
      QString bsaName = archives.at(i).trimmed();
      if (GameInfo::instance().isInvalidationBSA(ToWString(bsaName))) {
        return true;
      }
    }
  } else {
    *supported = false;
  }
  return false;
}


void Profile::deactivateInvalidation() const
{
  if (GameInfo::instance().requiresBSAInvalidation()) {
    wchar_t buffer[1024];
    std::wstring iniFileName = ToWString(QDir::toNativeSeparators(getIniFileName()));
    errno = 0;
    if (::GetPrivateProfileStringW(L"Archive", GameInfo::instance().archiveListKey().c_str(),
                                   L"", buffer, 1024, iniFileName.c_str()) == 0) {
      if (errno == 0x02) {
        QString errorMessage = tr("failed to parse ini file (%1): %2").arg(QDir::toNativeSeparators(getIniFileName())).arg(::GetLastError());
        throw windows_error(errorMessage.toUtf8().constData());
      } else {
        return;
      }
    }
    QStringList archives = ToQString(buffer).split(", ");

    for (int i = 0; i < archives.count();) {
      QString bsaName = archives.at(i).trimmed();
      if (GameInfo::instance().isInvalidationBSA(ToWString(bsaName))) {
        archives.removeAt(i);
      } else {
        ++i;
      }
    }

    // just to be safe...
    ::SetFileAttributesW(iniFileName.c_str(), FILE_ATTRIBUTE_NORMAL);

    if (!::WritePrivateProfileStringW(L"Archive", GameInfo::instance().archiveListKey().c_str(),
                                      ToWString(archives.join(", ").toUtf8()).c_str(), iniFileName.c_str()) ||
        !::WritePrivateProfileStringW(L"Archive", L"bInvalidateOlderFiles", L"0", iniFileName.c_str()) ||
        !::WritePrivateProfileStringW(L"Archive", L"SInvalidationFile", L"ArchiveInvalidation.txt", iniFileName.c_str())) {
      QString errorMessage = tr("failed to modify \"%1\"").arg(ToQString(iniFileName));
      throw windows_error(errorMessage.toUtf8().constData());
    }
  }
}


void Profile::activateInvalidation(const QString& dataDirectory) const
{
  if (GameInfo::instance().requiresBSAInvalidation()) {
    wchar_t buffer[1024];
    std::wstring iniFileName = ToWString(QDir::toNativeSeparators(getIniFileName()));
    errno = 0;
    if (::GetPrivateProfileStringW(L"Archive", GameInfo::instance().archiveListKey().c_str(),
                                   L"", buffer, 1024, iniFileName.c_str()) == 0) {
      if (errno == 0x02) {
        throw windows_error("failed to parse ini file");
      } else {
        // ignore. shouldn't have gotten here anyway
        return;
      }
    }
    QStringList archives = ToQString(buffer).split(", ");

    QString invalidationBSA = ToQString(GameInfo::instance().getInvalidationBSA());

    if (!archives.contains(invalidationBSA)) {
      archives.insert(0, invalidationBSA);
    }

    // just to be safe...
    ::SetFileAttributesW(iniFileName.c_str(), FILE_ATTRIBUTE_NORMAL);

    if (!::WritePrivateProfileStringW(L"Archive", GameInfo::instance().archiveListKey().c_str(),
                                      ToWString(archives.join(", ").toUtf8()).c_str(), iniFileName.c_str()) ||
        !::WritePrivateProfileStringW(L"Archive", L"bInvalidateOlderFiles", L"1", iniFileName.c_str()) ||
        !::WritePrivateProfileStringW(L"Archive", L"SInvalidationFile", L"", iniFileName.c_str())) {
      QString errorMessage = tr("failed to modify \"%1\"").arg(ToQString(iniFileName));
      throw windows_error(errorMessage.toUtf8().constData());
    }

    QString bsaFile = dataDirectory + "/" + invalidationBSA;
    if (!QFile::exists(bsaFile)) {
      DummyBSA bsa;
      bsa.write(bsaFile);
    }
  }
}


bool Profile::localSavesEnabled() const
{
  return m_Directory.exists("saves");
}


bool Profile::enableLocalSaves(bool enable)
{
  if (enable) {
    if (m_Directory.exists("_saves")) {
      m_Directory.rename("_saves", "saves");
    } else {
      m_Directory.mkdir("saves");
    }
  } else {
    QMessageBox::StandardButton res = QMessageBox::question(QApplication::activeModalWidget(), tr("Delete savegames?"),
                                                            tr("Do you want to delete local savegames? (If you select \"No\", the save games "
                                                               "will show up again if you re-enable local savegames)"),
                                                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                                            QMessageBox::Cancel);
    if (res == QMessageBox::Yes) {
      shellDelete(QStringList(m_Directory.absoluteFilePath("saves")));
    } else if (res == QMessageBox::No) {
      m_Directory.rename("saves", "_saves");
    } else {
      return false;
    }
  }

  // default: assume success
  return true;
}


QString Profile::getModlistFileName() const
{
  return QDir::cleanPath(m_Directory.absoluteFilePath("modlist.txt"));
}

QString Profile::getPluginsFileName() const
{
  return QDir::cleanPath(m_Directory.absoluteFilePath("plugins.txt"));
}

QString Profile::getLoadOrderFileName() const
{
  return QDir::cleanPath(m_Directory.absoluteFilePath("loadorder.txt"));
}


QString Profile::getLockedOrderFileName() const
{
  return QDir::cleanPath(m_Directory.absoluteFilePath("lockedorder.txt"));
}


QString Profile::getArchivesFileName() const
{
  return QDir::cleanPath(m_Directory.absoluteFilePath("archives.txt"));
}

QString Profile::getDeleterFileName() const
{
  return QDir::cleanPath(m_Directory.absoluteFilePath("hide_plugins.txt"));
}


QString Profile::getIniFileName() const
{
  std::wstring primaryIniFile = *(GameInfo::instance().getIniFileNames().begin());
  return m_Directory.absoluteFilePath(ToQString(primaryIniFile));
}

QString Profile::getPath() const
{
  return QDir::cleanPath(m_Directory.absolutePath());
}

void Profile::rename(const QString &newName)
{
  QDir profileDir(QDir::fromNativeSeparators(ToQString(GameInfo::instance().getProfilesDir())));
  profileDir.rename(getName(), newName);
  m_Directory = profileDir.absoluteFilePath(newName);
}
