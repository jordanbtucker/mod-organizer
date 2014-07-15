/*
Mod Organizer shared UI functionality

Copyright (C) 2012 Sebastian Herbord. All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef TUTORIALCONTROL_H
#define TUTORIALCONTROL_H


#include "dllimport.h"
#include <QWidget>
#if QT_VERSION >= 0x050000
#include <QQuickView>
#else
#include <QDeclarativeView>
#endif
#include <QScriptEngine>
#include <utility>

namespace MOBase {

class TutorialManager;

class QDLLEXPORT TutorialControl : public QObject
{
  Q_OBJECT
public:

  TutorialControl(const TutorialControl &reference);
  TutorialControl(QWidget *targetControl, const QString &name);

  ~TutorialControl();

  void registerControl();
  void resize(const QSize &size);

  void expose(const QString &widgetName, QObject *widget);

  void startTutorial(const QString &tutorial);

  Q_INVOKABLE void finish();
  Q_INVOKABLE QRect getRect(const QString &widgetName);
  Q_INVOKABLE QRect getActionRect(const QString &widgetName);
  Q_INVOKABLE QWidget *getChild(const QString &name);
  Q_INVOKABLE bool waitForButton(const QString &buttonName);
  Q_INVOKABLE bool waitForAction(const QString &actionName);
  Q_INVOKABLE bool waitForTabOpen(const QString &tabControlName, int tab);
  Q_INVOKABLE void lockUI(bool locked);
private slots:

  void tabChangedProxy(int selected);
  void nextTutorialStepProxy();

private:

  QWidget *m_TargetControl;
  QString m_Name;
#if QT_VERSION >= 0x050000
  QQuickView *m_TutorialView;
#else
  QDeclarativeView *m_TutorialView;
#endif
  QWidget *m_TutorialWidget;

  TutorialManager &m_Manager;

  std::vector<std::pair<QString, QObject*> > m_ExposedObjects;
  int m_ExpectedTab;
  QWidget *m_CurrentClickControl;

};

} // namespace MOBase

#endif // TUTORIALCONTROL_H
