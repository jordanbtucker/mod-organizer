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

#include "downloadlist.h"
#include "downloadmanager.h"
#include <QEvent>

#include <QSortFilterProxyModel>


DownloadList::DownloadList(DownloadManager *manager, QObject *parent)
  : QAbstractTableModel(parent), m_Manager(manager)
{
  connect(m_Manager, SIGNAL(update(int)), this, SLOT(update(int)));
  connect(m_Manager, SIGNAL(aboutToUpdate()), this, SLOT(aboutToUpdate()));
}


int DownloadList::rowCount(const QModelIndex&) const
{
  return m_Manager->numTotalDownloads() + m_Manager->numPendingDownloads();
}


int DownloadList::columnCount(const QModelIndex&) const
{
  return 3;
}


QModelIndex DownloadList::index(int row, int column, const QModelIndex&) const
{
  return createIndex(row, column, row);
}


QModelIndex DownloadList::parent(const QModelIndex&) const
{
  return QModelIndex();
}


QVariant DownloadList::headerData(int section, Qt::Orientation orientation, int role) const
{
  if ((role == Qt::DisplayRole) &&
      (orientation == Qt::Horizontal)) {
    switch (section) {
      case COL_NAME: return tr("Name");
      case COL_FILETIME: return tr("Filetime");
      default: return tr("Done");
    }
  } else {
    return QAbstractItemModel::headerData(section, orientation, role);
  }
}


QVariant DownloadList::data(const QModelIndex &index, int role) const
{
  if (role == Qt::DisplayRole) {
    return index.row();
  } else if (role == Qt::ToolTipRole) {
    if (index.row() < m_Manager->numTotalDownloads()) {
      QString text = m_Manager->getFileName(index.row()) + "\n";
      if (m_Manager->isInfoIncomplete(index.row())) {
        text += tr("Information missing, please select \"Query Info\" from the context menu to re-retrieve.");
      } else {
      const MOBase::ModRepositoryFileInfo *info = m_Manager->getFileInfo(index.row());
      return QString("%1 (ID %2) %3<br><span>%4</span>").arg(info->modName).arg(m_Manager->getModID(index.row())).arg(info->version.canonicalString()).arg(info->description);
      }
      return text;
    } else {
      return tr("pending download");
    }
  } else {
    return QVariant();
  }
}


void DownloadList::aboutToUpdate()
{
  emit beginResetModel();
}


void DownloadList::update(int row)
{
  if (row < 0) {
    emit endResetModel();
  } else if (row < this->rowCount()) {
#pragma message("updating only the one column is a hack")
    emit dataChanged(this->index(row, 2, QModelIndex()), this->index(row, 2, QModelIndex()));
  } else {
    qCritical("invalid row %d in download list, update failed", row);
  }
}

