/*
Copyright (C) 2014 Sebastian Herbord. All rights reserved.

This file is part of DDS Preview plugin for MO

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


#ifndef PREVIEWDDS_H
#define PREVIEWDDS_H


#include <ipluginpreview.h>
#include <QGLWidget>


class PreviewDDS : public MOBase::IPluginPreview
{

  Q_OBJECT
  Q_INTERFACES(MOBase::IPlugin MOBase::IPluginPreview)
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
  Q_PLUGIN_METADATA(IID "org.tannin.PreviewDDS" FILE "previewdds.json")
#endif

public:
  PreviewDDS();

public: // IPlugin interface
  virtual bool init(MOBase::IOrganizer *moInfo);
  virtual QString name() const;
  virtual QString author() const;
  virtual QString description() const;
  virtual MOBase::VersionInfo version() const;
  virtual bool isActive() const;
  virtual QList<MOBase::PluginSetting> settings() const;

public: // IPluginPreview interface
  virtual std::set<QString> supportedExtensions() const;
  virtual QWidget *genFilePreview(const QString &fileName, const QSize &maxSize) const;

private:

  mutable QGLWidget m_GLWidget;
  GLuint m_Id;

};

#endif // PREVIEWDDS_H
