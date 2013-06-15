#include "diagnosebasic.h"
#include <report.h>
#include <utility.h>
#include <QtPlugin>
#include <QFile>
#include <QDir>
#include <QCoreApplication>


using namespace MOBase;


DiagnoseBasic::DiagnoseBasic()
{
}

bool DiagnoseBasic::init(IOrganizer *moInfo)
{
  m_MOInfo = moInfo;
  return true;
}

QString DiagnoseBasic::name() const
{
  return tr("Basic diagnosis plugin");
}

QString DiagnoseBasic::author() const
{
  return "Tannin";
}

QString DiagnoseBasic::description() const
{
  return tr("Checks for problems unrelated to other plugins");
}

VersionInfo DiagnoseBasic::version() const
{
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
}

bool DiagnoseBasic::isActive() const
{
  return true;
}

QList<PluginSetting> DiagnoseBasic::settings() const
{
  return QList<PluginSetting>();
}


bool DiagnoseBasic::errorReported() const
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


bool DiagnoseBasic::overwriteFiles() const
{
  QDir dir(QCoreApplication::applicationDirPath() + "/overwrite");
  return dir.count() != 2; // account for . and ..
}


std::vector<unsigned int> DiagnoseBasic::activeProblems() const
{
  std::vector<unsigned int> result;

  if (errorReported()) {
    result.push_back(PROBLEM_ERRORLOG);
  }
  if (overwriteFiles()) {
    result.push_back(PROBLEM_OVERWRITE);
  }

  return result;
}

QString DiagnoseBasic::shortDescription(unsigned int key) const
{
  switch (key) {
    case PROBLEM_ERRORLOG:
      return tr("There was an error reported recently");
    case PROBLEM_OVERWRITE:
      return tr("There are files in your overwrite mod");
    default:
      throw MyException(tr("invalid problem key %1").arg(key));
  }
}

QString DiagnoseBasic::fullDescription(unsigned int key) const
{
  switch (key) {
    case PROBLEM_ERRORLOG:
      return "<code>" + m_ErrorMessage.replace("\n", "<br>") + "</code>";
    case PROBLEM_OVERWRITE:
      return tr("Files in the <font color=\"red\"><i>Overwrite</i></font> mod are are usually files created by a foreign tool (i.e. Wrye Bash, Automatic Variants, ...).<br>"
                "For technical reasons it's not possible to assign those files to the corresponding mod automatically, you have to deal with these "
                "files manually.<br>Use the right-click menu on <font color=\"red\"><i>Overwrite</i></font> to create a regular mod from all the files currently in that folder or double "
                "click it and then drag&drop files to an existing mod.");
    default:
      throw MyException(tr("invalid problem key %1").arg(key));
  }
}

bool DiagnoseBasic::hasGuidedFix(unsigned int) const
{
  return false;
}

void DiagnoseBasic::startGuidedFix(unsigned int key) const
{
  throw MyException(tr("invalid problem key %1").arg(key));
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(diagnosebasic, DiagnoseBasic)
#endif
