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


#include "previewdds.h"
#include <boost/assign.hpp>
#include <QGLPixelBuffer>
#include <QLabel>
#include <QtPlugin>

// dependencies of gltexloaders
#include <QModelIndex>
#include "gl/gltexloaders.h"

using namespace MOBase;


PreviewDDS::PreviewDDS()
{
}


std::set<QString> PreviewDDS::supportedExtensions() const
{
  return boost::assign::list_of("dds");
}

bool PreviewDDS::init(MOBase::IOrganizer*)
{
  glGenTextures(1, &m_Id);
  return true;
}

QString PreviewDDS::name() const
{
  return tr("DDS Preview");
}

QString PreviewDDS::author() const
{
  return "Tannin";
}

QString PreviewDDS::description() const
{
  return tr("Provides file previews for .dds files. Based on dds loading code from the niftools project: ") + "<a href=\"https://sourceforge.net/projects/niftools/\">https://sourceforge.net/projects/niftools/</a>";
}

MOBase::VersionInfo PreviewDDS::version() const
{
  return VersionInfo(0, 1, 0, VersionInfo::RELEASE_BETA);
}

bool PreviewDDS::isActive() const
{
  return true;
}

QList<MOBase::PluginSetting> PreviewDDS::settings() const
{
  return QList<MOBase::PluginSetting>();
}


QWidget *PreviewDDS::genFilePreview(const QString &fileName, const QSize &maxSize) const
{
  m_GLWidget.makeCurrent();

  GLuint width, height, mipmaps;
  width = height = mipmaps = 0;
  QString format;

  glBindTexture(GL_TEXTURE_2D, m_Id);
  try {
    texLoad(fileName, format, width, height, mipmaps);
  } catch (const QString &e) {
    qWarning("failed to load dds texture: %s", qPrintable(e));
    return nullptr;
  }

  QGLPixelBuffer pbuffer(QSize(width, height), m_GLWidget.format(), &m_GLWidget);
  if (!pbuffer.makeCurrent()) {
    qWarning("failed to activate pixel buffer");
    return nullptr;
  }

  pbuffer.drawTexture(QRectF(-1, -1, 2, 2), m_Id);

  QLabel *label = new QLabel();
  QImage image = pbuffer.toImage();
  if ((image.size().width() > maxSize.width()) ||
      (image.size().height() > maxSize.height())) {
    image = image.scaled(maxSize, Qt::KeepAspectRatio);
  }

  label->setPixmap(QPixmap::fromImage(image));
  return label;
}


#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(previewDDS, PreviewDDS)
#endif
