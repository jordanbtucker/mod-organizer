/*
Copyright (C) 2013 Sebastian Herbord. All rights reserved.

This file is part of the bsa order diagnosis plugin for Mod Organizer

This plugin is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This plugin is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this plugin.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "diagnosebsaorder.h"
#include "diagnosebasic.h"
#include <QtPlugin>


using namespace MOBase;


DiagnoseBSAOrder::DiagnoseBSAOrder()
{
}


bool DiagnoseBSAOrder::init(IOrganizer *moInfo)
{
  m_MOInfo = moInfo;
  return true;
}

QString DiagnoseBSAOrder::name() const
{
  return tr("Basic diagnosis plugin");
}

QString DiagnoseBSAOrder::author() const
{
  return "Tannin";
}

QString DiagnoseBSAOrder::description() const
{
  return tr("Checks for problems unrelated to other plugins");
}

VersionInfo DiagnoseBSAOrder::version() const
{
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
}

bool DiagnoseBSAOrder::isActive() const
{
  return true;
}

QList<PluginSetting> DiagnoseBSAOrder::settings() const
{
  return QList<PluginSetting>();
}


bool DiagnoseBSAOrder::errorReported() const
{
  QDir dir(QCoreApplication::applicationDirPath() + "/logs");
  QFileInfoList files = dir.entryInfoList(QStringList("ModOrganizer_??_??_??_??_??.log"),
                                          QDir::Files, QDir::Name | QDir::Reversed);

  if (files.count() > 0) {
    QString logFile = files.at(0).absoluteFilePath();
    QFile file(logFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      char buffer[1024];
      int line = 0;
      qint64 lineLengths[NUM_CONTEXT_ROWS];
      for (int i = 0; i < NUM_CONTEXT_ROWS; ++i) {
        lineLengths[i] = 0;
      }
      while (!file.atEnd()) {
        lineLengths[line % NUM_CONTEXT_ROWS] = file.readLine(buffer, 1024) + 1;
        if (strncmp(buffer, "ERROR", 5) == 0) {
          qint64 sumChars = 0;
          for (int i = 0; i < NUM_CONTEXT_ROWS; ++i) {
            sumChars += lineLengths[i];
          }
          file.seek(file.pos() - sumChars);
          m_ErrorMessage = "";
          for (int i = 0; i < 2 * NUM_CONTEXT_ROWS; ++i) {
            file.readLine(buffer, 1024);
            QString lineString = QString::fromUtf8(buffer);
            if (lineString.startsWith("ERROR")) {
              m_ErrorMessage += "<b>" + lineString + "</b>";
            } else {
              m_ErrorMessage += lineString;
            }
          }
          return true;
        }

        // prevent this function from taking forever
        if (line++ >= 50000) {
          break;
        }
      }
    }
  }

  return false;
}


bool DiagnoseBSAOrder::overwriteFiles() const
{
  QDir dir(QCoreApplication::applicationDirPath() + "/overwrite");
  return dir.count() != 2; // account for . and ..
}

bool DiagnoseBSAOrder::nitpickInstalled() const
{
  QString path = m_MOInfo->resolvePath("skse/plugins/nitpick.dll");
  return !path.isEmpty();
}

bool DiagnoseBSAOrder::invalidFontConfig() const
{
  if (m_MOInfo->gameInfo().type() != IGameInfo::TYPE_SKYRIM) {
    // this check is only for skyrim
    return false;
  }

  // files from skyrim_interface.bsa
  static std::vector<QString> defaultFonts = boost::assign::list_of("interface\\fonts_console.swf")
                                                                   ("interface\\fonts_en.swf");

  QString configPath = m_MOInfo->resolvePath("interface/fontconfig.txt");
  if (configPath.isEmpty()) {
    return false;
  }
  QFile config(configPath);
  if (!config.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug("failed to open %s", qPrintable(configPath));
    return false;
  }

  std::tr1::regex exp("^fontlib \"([^\"]*)\"$");
  while (!config.atEnd()) {
    QByteArray row = config.readLine();
    std::tr1::cmatch match;
    if (std::tr1::regex_search(row.constData(), match, exp)) {
      std::string temp = match[1];
      QString path(temp.c_str());
      bool isDefault = false;
      foreach (const QString &def, defaultFonts) {
        if (QString::compare(def, path, Qt::CaseInsensitive) == 0) {
          isDefault = true;
          break;
        }
      }

      if (!isDefault && m_MOInfo->resolvePath(path).isEmpty()) {
        return true;
      }
    }
  }
  return false;
}


std::vector<unsigned int> DiagnoseBSAOrder::activeProblems() const
{
  std::vector<unsigned int> result;

  if (bsaOrderMismatch()) {
    result.push_back(1);
  }

  return result;
}

QString DiagnoseBSAOrder::shortDescription(unsigned int) const
{
  return tr("BSA ordering may be wrong.");
}

QString DiagnoseBSAOrder::fullDescription(unsigned int) const
{
  return tr("Some of your BSAs are ordered differently than they would if you were using a different Mod Management system. It's advisable you order "
            "mods containing a BSA the same way as the esps they contain.");
}

bool DiagnoseBSAOrder::hasGuidedFix(unsigned int) const
{
  return false;
}

void DiagnoseBSAOrder::startGuidedFix(unsigned int key) const
{
  throw MyException(tr("invalid problem key %1").arg(key));
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(diagnosebsaorder, DiagnoseBSAOrder)
#endif
