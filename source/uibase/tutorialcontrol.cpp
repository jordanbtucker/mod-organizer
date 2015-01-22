/*
Copyright (C) 2012 Sebastian Herbord. All rights reserved.

This file is part of Mod Organizer.

Mod Organizer is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Mod Organizer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Mod Organizer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tutorialcontrol.h"
#include <QCoreApplication>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QMouseEvent>
#include <QToolBar>
#include <QAction>
#include <QGraphicsObject>
#include <QTimer>
#include <QAbstractButton>
#include <QTableWidget>
#include "utility.h"
#include "tutorialmanager.h"
#include "report.h"
#include <boost/scoped_array.hpp>
#include <QImage>
#include <QBitmap>



#include <QMetaObject>
#include <QMetaMethod>
#include <QMetaEnum>
#include <qmetaobject.h>


namespace MOBase {


TutorialControl::TutorialControl(const TutorialControl &reference)
  : QObject(reference.parent())
  , m_TargetControl(reference.m_TargetControl)
  , m_Name(reference.m_Name)
  , m_TutorialView(NULL)
  , m_Manager(TutorialManager::instance())
  , m_ExpectedTab(0)
  , m_CurrentClickControl(NULL)
{
}


TutorialControl::TutorialControl(QWidget *targetControl, const QString &name)
  : QObject(NULL)
  , m_TargetControl(targetControl)
  , m_Name(name)
  , m_TutorialView(NULL)
  , m_Manager(TutorialManager::instance())
  , m_ExpectedTab(0)
  , m_CurrentClickControl(NULL)
{
}


TutorialControl::~TutorialControl()
{
  m_Manager.unregisterControl(m_Name);
  finish();
}


void TutorialControl::registerControl()
{
  m_Manager.registerControl(m_Name, this);
}


void TutorialControl::resize(const QSize &size)
{
  if (m_TutorialView != NULL) {
    m_TutorialView->resize(size.width(), size.height());
  }
}


void TutorialControl::expose(const QString &widgetName, QObject *widget)
{
  m_ExposedObjects.push_back(std::make_pair(widgetName, widget));
}


static QString canonicalPath(const QString &path)
{
  boost::scoped_array<wchar_t> buffer(new wchar_t[32768]);
  DWORD res = ::GetShortPathNameW((wchar_t*)path.utf16(), buffer.get(), 32768);
  if (res == 0) {
    return path;
  }
  res = ::GetLongPathNameW(buffer.get(), buffer.get(), 32768);
  if (res == 0) {
    return path;
  }
  return QString::fromWCharArray(buffer.get());
}

void TutorialControl::startTutorial(const QString &tutorial)
{
  if (m_TutorialView == NULL) {
    m_TutorialView = new QDeclarativeView(m_TargetControl);
    m_TutorialView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    m_TutorialView->setStyleSheet(QString("background: transparent"));
    m_TutorialView->setObjectName("tutorialView");

    QString qmlName = canonicalPath(QCoreApplication::applicationDirPath() + "/tutorials") + "/tutorials_" + m_Name.toLower() + ".qml";
    QUrl qmlSource = QUrl::fromLocalFile(qmlName);

    m_TutorialView->setSource(qmlSource);
    m_TutorialView->resize(m_TargetControl->width(), m_TargetControl->height());
    m_TutorialView->rootContext()->setContextProperty("scriptName", tutorial);
    m_TutorialView->rootContext()->setContextProperty("tutorialControl", this);
    m_TutorialView->rootContext()->setContextProperty("manager", &m_Manager);
    m_TutorialView->rootContext()->setContextProperty("applicationWindow", m_TargetControl);

    for (std::vector<std::pair<QString, QObject*> >::const_iterator iter = m_ExposedObjects.begin();
         iter != m_ExposedObjects.end(); ++iter) {
      m_TutorialView->rootContext()->setContextProperty(iter->first, iter->second);
    }
    m_TutorialView->show();
    m_TutorialView->raise();
    if (!QMetaObject::invokeMethod(m_TutorialView->rootObject(), "init")) {
      reportError(tr("Tutorial failed to start, please check \"mo_interface.log\" for details."));
      m_TutorialView->close();
    }
  }
}


void TutorialControl::lockUI(bool locked)
{
  m_TutorialView->setAttribute(Qt::WA_TransparentForMouseEvents, !locked);

  QMetaObject::invokeMethod(m_TutorialView->rootObject(), "enableBackground", Q_ARG(QVariant, QVariant(locked)));
}


QWidget* TutorialControl::getChild(const QString &name)
{
  if (m_TargetControl != NULL) {
    return m_TargetControl->findChild<QWidget*>(name);
  } else {
    return NULL;
  }
}


void TutorialControl::finish()
{
  if (m_TutorialView != NULL) {
    m_TutorialView->deleteLater();
  }
  m_TutorialView = NULL;
  m_TutorialView = NULL;
}


QRect TutorialControl::getRect(const QString &widgetName)
{
  if (m_TargetControl != NULL) {
    QWidget *widget = m_TargetControl->findChild<QWidget*>(widgetName);
    if (widget != NULL) {
      QRect res = widget->rect();
      QPoint pos = widget->mapTo(m_TargetControl, res.topLeft());
      res.moveTopLeft(pos);
      return res;
    } else {
      qCritical("%s not found", widgetName.toUtf8().constData());
      return QRect();
    }
  } else {
    return QRect();
  }
}


QRect TutorialControl::getActionRect(const QString &widgetName)
{
  if (m_TargetControl != NULL) {
    QToolBar *toolBar = m_TargetControl->findChild<QToolBar*>("toolBar");
    foreach (QAction *action, toolBar->actions()) {
      if (action->objectName() == widgetName) {
        return toolBar->actionGeometry(action);
      }
    }
  }
  return QRect();
}

void TutorialControl::nextTutorialStepProxy()
{

  if (m_TutorialView != NULL) {
    QObject *background = m_TutorialView->rootObject();

    QTimer::singleShot(1, background, SLOT(nextStep()));
    lockUI(true);

    bool success = false;
    if (sender()->inherits("QAction")) {
      success = disconnect(sender(), SIGNAL(triggered()),
                           this, SLOT(nextTutorialStepProxy()));
    } else {
      success = disconnect(sender(), SIGNAL(pressed()),
                           this, SLOT(nextTutorialStepProxy()));
    }
    if (!success) {
      qCritical("failed to disconnect tutorial proxy");
    }
  } else {
    qCritical("failed to proceed to next tutorial step");
    finish();
  }
}


void TutorialControl::tabChangedProxy(int selected)
{
  if ((m_TutorialView != NULL) && (selected == m_ExpectedTab)) {
    QObject *background = m_TutorialView->rootObject();
    QTimer::singleShot(1, background, SLOT(nextStep()));
    lockUI(true);
    if (!disconnect(sender(), SIGNAL(currentChanged(int)),
                    this, SLOT(tabChangedProxy(int)))) {
      qCritical("failed to disconnect tab-changed proxy");
    }
  }
}


bool TutorialControl::waitForAction(const QString &actionName)
{
  if (m_TargetControl != NULL) {
    QAction *action = m_TargetControl->findChild<QAction*>(actionName);
    if (action == NULL) {
      qCritical("no action \"%s\" in control \"%s\"",
                actionName.toUtf8().constData(), m_Name.toUtf8().constData());
      return false;
    }
    if (action->isEnabled()) {
      connect(action, SIGNAL(triggered()), this, SLOT(nextTutorialStepProxy()));
      lockUI(false);
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool TutorialControl::waitForButton(const QString &buttonName)
{
  if (m_TargetControl != NULL) {
    QAbstractButton *button = m_TargetControl->findChild<QAbstractButton*>(buttonName);
    if (button == NULL) {
      qCritical("no button \"%s\" in control \"%s\"",
                buttonName.toUtf8().constData(), m_Name.toUtf8().constData());
      return false;
    }
    if (button->isEnabled()) {
      connect(button, SIGNAL(pressed()), this, SLOT(nextTutorialStepProxy()));
      lockUI(false);
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}


bool TutorialControl::waitForTabOpen(const QString &tabControlName, int tab)
{
  if (m_TargetControl != NULL) {
    QTabWidget *tabWidget = m_TargetControl->findChild<QTabWidget*>(tabControlName);
    if (tabWidget == NULL) {
      qCritical("no tab widget \"%s\" in control \"%s\"",
                tabControlName.toUtf8().constData(), m_Name.toUtf8().constData());
      return false;
    }
    if (tabWidget->isEnabled() && (tabWidget->currentIndex() != tab)) {
      m_ExpectedTab = tab;
      connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChangedProxy(int)));
      lockUI(false);
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}
} // namespace MOBase
