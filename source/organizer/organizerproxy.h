#ifndef ORGANIZERPROXY_H
#define ORGANIZERPROXY_H


#include <imoinfo.h>
#include "mainwindow.h"

class OrganizerProxy : public MOBase::IOrganizer
{
public:
  OrganizerProxy(MainWindow *window, const QString &pluginName);

  virtual MOBase::IGameInfo &gameInfo() const;
  virtual MOBase::IModRepositoryBridge *createNexusBridge() const;
  virtual QString profileName() const;
  virtual QString profilePath() const;
  virtual QString downloadsPath() const;
  virtual QString overwritePath() const;
  virtual MOBase::VersionInfo appVersion() const;
  virtual MOBase::IModInterface *getMod(const QString &name);
  virtual MOBase::IModInterface *createMod(MOBase::GuessedValue<QString> &name);
  virtual bool removeMod(MOBase::IModInterface *mod);
  virtual void modDataChanged(MOBase::IModInterface *mod);
  virtual QVariant pluginSetting(const QString &pluginName, const QString &key) const;
  virtual void setPluginSetting(const QString &pluginName, const QString &key, const QVariant &value);
  virtual QVariant persistent(const QString &pluginName, const QString &key, const QVariant &def = QVariant()) const;
  virtual void setPersistent(const QString &pluginName, const QString &key, const QVariant &value, bool sync = true);
  virtual QString pluginDataPath() const;
  virtual MOBase::IModInterface *installMod(const QString &fileName);
  virtual QString resolvePath(const QString &fileName) const;
  virtual QStringList listDirectories(const QString &directoryName) const;
  virtual QStringList findFiles(const QString &path, const std::function<bool(const QString &)> &filter) const;
  virtual QStringList getFileOrigins(const QString &fileName) const;
  virtual QList<FileInfo> findFileInfos(const QString &path, const std::function<bool(const FileInfo&)> &filter) const;

  virtual MOBase::IDownloadManager *downloadManager();
  virtual MOBase::IPluginList *pluginList();
  virtual MOBase::IModList *modList();
  virtual HANDLE startApplication(const QString &executable, const QStringList &args = QStringList(), const QString &cwd = "", const QString &profile = "");
  virtual bool waitForApplication(HANDLE handle, LPDWORD exitCode = NULL) const;
  virtual void refreshModList(bool saveChanges);

  virtual bool onAboutToRun(const std::function<bool(const QString&)> &func);
  virtual bool onModInstalled(const std::function<void (const QString &)> &func);

private:
  MainWindow *m_Proxied;
  const QString &m_PluginName;

};


#endif // ORGANIZERPROXY_H
