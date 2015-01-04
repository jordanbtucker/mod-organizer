#include "executableinfo.h"

using namespace MOBase;


MOBase::ExecutableInfo::ExecutableInfo(const QString &title, const QFileInfo &binary)
  : m_Title(title)
  , m_Binary(binary)
  , m_WorkingDirectory(binary.absoluteDir())
  , m_CloseMO(CloseMOStyle::DEFAULT_STAY)
  , m_SteamAppID()
{
}

ExecutableInfo &ExecutableInfo::withArgument(const QString &argument)
{
  m_Arguments.append(argument);
  return *this;
}

ExecutableInfo &ExecutableInfo::withWorkingDirectory(const QDir &workingDirectory)
{
  m_WorkingDirectory = workingDirectory;
  return *this;
}

ExecutableInfo &MOBase::ExecutableInfo::withSteamAppId(const QString &appId)
{
  m_SteamAppID = appId;
  return *this;
}

ExecutableInfo &ExecutableInfo::withDefaultClose()
{
  m_CloseMO = CloseMOStyle::DEFAULT_CLOSE;
  return *this;
}

ExecutableInfo &ExecutableInfo::withNeverClose()
{
  m_CloseMO = CloseMOStyle::NEVER_CLOSE;
  return *this;
}

ExecutableInfo &ExecutableInfo::asCustom()
{
  m_Custom = true;
  return *this;
}

void ExecutableInfo::showInToolbar(bool show)
{
  m_ShowInToolbar = show;
}

QString ExecutableInfo::title() const
{
  return m_Title;
}

QFileInfo ExecutableInfo::binary() const
{
  return m_Binary;
}

QStringList ExecutableInfo::arguments() const
{
  return m_Arguments;
}

QDir ExecutableInfo::workingDirectory() const
{
  return m_WorkingDirectory;
}

ExecutableInfo::CloseMOStyle ExecutableInfo::closeMO() const
{
  return m_CloseMO;
}

QString ExecutableInfo::steamAppID() const
{
  return m_SteamAppID;
}

bool ExecutableInfo::isCustom() const
{
  return m_Custom;
}

bool ExecutableInfo::showInToolbar() const
{
  return m_ShowInToolbar;
}
