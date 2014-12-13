#ifndef ABOUTDIALOG_H
/*
Copyright (C) 2014 Sebastian Herbord. All rights reserved.

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


#define ABOUTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <map>
#include <vector>
#include <utility>

namespace Ui {
  class AboutDialog;
}

class AboutDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AboutDialog(const QString &version, QWidget *parent = 0);
  ~AboutDialog();

private:

  enum Licenses {
    LICENSE_NONE,
    LICENSE_LGPL3,
    LICENSE_GPL3,
    LICENSE_BSD3,
    LICENSE_BOOST,
    LICENSE_CCBY3,
    LICENSE_ZLIB,
    LICENSE_APACHE2
  };

private:

  void addLicense(const QString &name, Licenses license);

private slots:
  void on_creditsList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:

  Ui::AboutDialog *ui;

  std::map<int, QString> m_LicenseFiles;

};

#endif // ABOUTDIALOG_H
