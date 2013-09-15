#include "pagetesalliance.h"
#include <versioninfo.h>
#include <regex>
#include <boost/lexical_cast.hpp>
#include <QUrl>

#include <QtPlugin>


using namespace MOBase;


PageTESAlliance::PageTESAlliance()
{
}

bool PageTESAlliance::init(MOBase::IOrganizer*)
{
  return true;
}

QString PageTESAlliance::name() const
{
  return "TES Alliance";
}

QString PageTESAlliance::author() const
{
  return "Tannin";
}

QString PageTESAlliance::description() const
{
  return "Adds support for downloading from TES Alliance";
}

VersionInfo PageTESAlliance::version() const
{
  return VersionInfo(0, 1, 0, VersionInfo::RELEASE_ALPHA);
}

bool PageTESAlliance::isActive() const
{
  return true;
}

QList<PluginSetting> PageTESAlliance::settings() const
{
  return QList<PluginSetting>();
}

QString PageTESAlliance::displayName() const
{
  return "TES Alliance";
}

QIcon PageTESAlliance::icon() const
{
  return QIcon();
}

QUrl PageTESAlliance::pageURL() const
{
  return QUrl("http://tesalliance.org");
}

QString PageTESAlliance::convertModName(const QString &inputName) const
{
  QString result = inputName;
  result.replace('-', ' ');
  return result;
}

bool PageTESAlliance::handlesDownload(const QUrl &pageURL, const QUrl &downloadURL, ModRepositoryFileInfo &fileInfo) const
{
  // match both page url and download url against the known pattern
  static std::tr1::regex pagePattern("http://tesalliance.org/forums/index.php\\?/files/download/([0-9]+)-([^/]+)/");
  static std::tr1::regex downloadPattern("http://tesalliance.org/forums/index.php\\?/files/getdownload/([0-9]+)-([^/]+)/");

  std::string pURL = pageURL.toString().toUtf8().constData();
  std::string dURL = downloadURL.toString().toUtf8().constData();
  std::tr1::match_results<std::string::const_iterator> pageMatch;
  std::tr1::match_results<std::string::const_iterator> downloadMatch;

  if (!std::tr1::regex_match(pURL, pageMatch, pagePattern) ||
      !std::tr1::regex_match(dURL, downloadMatch, downloadPattern)) {
    return false;
  }

  // if we got here the url is supported and we have a lot of useful information in the regex-match
  try {
    fileInfo.modID   = boost::lexical_cast<int>(pageMatch[1].str());
    fileInfo.fileID  = boost::lexical_cast<int>(downloadMatch[1].str());
  } catch (const boost::bad_lexical_cast &e) {
    qCritical("failed to parse tes alliance url: %s", e.what());
  }

  fileInfo.modName = convertModName(pageMatch[2].str().c_str());
  fileInfo.uri     = downloadURL.toString();

  return true;
}

bool PageTESAlliance::useIntegratedBrowser() const
{
  return true;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(pageTESAlliance, PageTESAlliance)
#endif
