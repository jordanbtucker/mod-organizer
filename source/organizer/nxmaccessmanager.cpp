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

#include "nxmaccessmanager.h"
#include "nxmurl.h"
#include "report.h"
#include "utility.h"
#include "selfupdater.h"
#include "persistentcookiejar.h"
#include <QMessageBox>
#include <QPushButton>
#include <QNetworkProxy>
#include <QNetworkRequest>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QCoreApplication>
#include <QDir>
#include <gameinfo.h>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QUrlQuery>
#endif

using namespace MOBase;
using namespace MOShared;


NXMAccessManager::NXMAccessManager(QObject *parent)
  : QNetworkAccessManager(parent)
  , m_LoginReply(NULL)
  , m_ProgressDialog()
  , m_LoginAttempted(false)
{
  setCookieJar(new PersistentCookieJar(
      QDir::fromNativeSeparators(MOBase::ToQString(MOShared::GameInfo::instance().getCacheDir())) + "/nexus_cookies.dat", this));
}

NXMAccessManager::~NXMAccessManager()
{
  if (m_LoginReply != NULL) {
    m_LoginReply->deleteLater();
    m_LoginReply = NULL;
  }
}


QNetworkReply *NXMAccessManager::createRequest(
    QNetworkAccessManager::Operation operation, const QNetworkRequest &request,
    QIODevice *device)
{
  if (request.url().scheme() != "nxm") {
    return QNetworkAccessManager::createRequest(operation, request, device);
  }
  if (operation == GetOperation) {
    emit requestNXMDownload(request.url().toString());

    // eat the request, everything else will be done by the download manager
    return QNetworkAccessManager::createRequest(QNetworkAccessManager::GetOperation,
                                                QNetworkRequest(QUrl()));
  } else if (operation == PostOperation) {
    return QNetworkAccessManager::createRequest(operation, request, device);;
  } else {
    return QNetworkAccessManager::createRequest(operation, request, device);
  }
}


void NXMAccessManager::showCookies()
{
  QList<QNetworkCookie> cookies = cookieJar()->cookiesForUrl(QUrl(ToQString(GameInfo::instance().getNexusPage()) + "/"));
  foreach (QNetworkCookie cookie, cookies) {
    qDebug("%s - %s", cookie.name().constData(), cookie.value().constData());
  }
}


bool NXMAccessManager::loggedIn() const
{
  return hasLoginCookies();
}

bool NXMAccessManager::loginWaiting() const
{
  return m_LoginReply != NULL;
}


void NXMAccessManager::login(const QString &username, const QString &password)
{
  if (m_LoginReply != NULL) {
    return;
  }

  if (hasLoginCookies()) {
    emit loginSuccessful(false);
    return;
  }

  m_LoginAttempted = true;

  m_Username = username;
  m_Password = password;
  pageLogin();
}


void NXMAccessManager::pageLogin()
{
  QString requestString = QString("http://gatekeeper.nexusmods.com/Sessions/?Login&uri=%1")
                             .arg(QString(QUrl::toPercentEncoding(ToQString(GameInfo::instance().getNexusPage()))));

  QNetworkRequest request(requestString);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

  QByteArray postDataQuery;
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  QUrlQuery postData;
  postData.addQueryItem("username", m_Username);
  postData.addQueryItem("password", m_Password);
  postDataQuery = postData.query(QUrl::FullyEncoded).toUtf8();
#else
  QUrl postData;
  postData.addQueryItem("username", m_Username);
  postData.addQueryItem("password", m_Password);
  postDataQuery = postData.encodedQuery();
#endif

  m_ProgressDialog.setLabelText(tr("Logging into Nexus"));
  QList<QPushButton*> buttons = m_ProgressDialog.findChildren<QPushButton*>();
  buttons.at(0)->setEnabled(false);
  m_ProgressDialog.show();
  QCoreApplication::processEvents(); // for some reason the whole app hangs during the login. This way the user has at least a little feedback

  m_LoginReply = post(request, postDataQuery);
  m_LoginTimeout.start();
  connect(m_LoginReply, SIGNAL(finished()), this, SLOT(loginFinished()));
  connect(m_LoginReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(loginError(QNetworkReply::NetworkError)));
}


void NXMAccessManager::loginTimeout()
{
  emit loginFailed(tr("timeout"));
  m_LoginReply->deleteLater();
  m_LoginReply = NULL;
  m_LoginAttempted = false; // this usually means we might have usccess later
  m_LoginTimeout.stop();
  m_Username.clear();
  m_Password.clear();
}


void NXMAccessManager::loginError(QNetworkReply::NetworkError)
{
  m_ProgressDialog.hide();
  emit loginFailed(m_LoginReply->errorString());
  m_LoginTimeout.stop();
  m_LoginReply->deleteLater();
  m_LoginReply = NULL;
  m_Username.clear();
  m_Password.clear();
}


bool NXMAccessManager::hasLoginCookies() const
{
  bool sidCookie = false;
  QList<QNetworkCookie> cookies = cookieJar()->cookiesForUrl(QUrl(ToQString(GameInfo::instance().getNexusPage()) + "/"));
  foreach (QNetworkCookie cookie, cookies) {
    if (cookie.name() == "sid") {
      sidCookie = true;
    }
  }
  return sidCookie;
}


void NXMAccessManager::loginFinished()
{
  m_ProgressDialog.hide();
  if (hasLoginCookies()) {
    emit loginSuccessful(true);
  } else {
    emit loginFailed(tr("Please check your password"));
  }

  m_LoginTimeout.stop();
  m_LoginReply->deleteLater();
  m_LoginReply = NULL;
  m_Username.clear();
  m_Password.clear();
}
