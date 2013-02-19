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

#include "downloadlistwidgetcompact.h"
#include "ui_downloadlistwidgetcompact.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include "downloadmanager.h"


DownloadListWidgetCompact::DownloadListWidgetCompact(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::DownloadListWidgetCompact)
{
  ui->setupUi(this);
}

DownloadListWidgetCompact::~DownloadListWidgetCompact()
{
  delete ui;
}


DownloadListWidgetCompactDelegate::DownloadListWidgetCompactDelegate(DownloadManager *manager, QTreeView *view, QObject *parent)
  : QItemDelegate(parent), m_Manager(manager), m_ItemWidget(new DownloadListWidgetCompact), m_View(view)
{
  m_NameLabel = m_ItemWidget->findChild<QLabel*>("nameLabel");
  m_Progress = m_ItemWidget->findChild<QProgressBar*>("downloadProgress");
  m_DoneLabel = m_ItemWidget->findChild<QLabel*>("doneLabel");

  m_DoneLabel->setVisible(false);
}


DownloadListWidgetCompactDelegate::~DownloadListWidgetCompactDelegate()
{
  delete m_ItemWidget;
}


void DownloadListWidgetCompactDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  try {
//    m_ItemWidget->resize(option.rect.size());
    m_ItemWidget->resize(QSize(m_View->columnWidth(0) + m_View->columnWidth(1) + m_View->columnWidth(2), option.rect.height()));
    if (index.row() % 2 == 1) {
      m_ItemWidget->setBackgroundRole(QPalette::AlternateBase);
    } else {
      m_ItemWidget->setBackgroundRole(QPalette::Base);
    }

    int downloadIndex = index.data().toInt();

    QString name = m_Manager->getFileName(downloadIndex);
    if (name.length() > 53) {
      name.truncate(50);
      name.append("...");
    }
    m_NameLabel->setText(name);
    DownloadManager::DownloadState state = m_Manager->getState(downloadIndex);
    if (state == DownloadManager::STATE_PAUSED) {
      m_DoneLabel->setVisible(true);
      m_Progress->setVisible(false);
      m_DoneLabel->setText(tr("Paused"));
      m_DoneLabel->setForegroundRole(QPalette::Link);
    } else if (state >= DownloadManager::STATE_READY) {
      m_DoneLabel->setVisible(true);
      m_Progress->setVisible(false);
      if (state == DownloadManager::STATE_INSTALLED) {
        m_DoneLabel->setText(tr("Installed"));
        m_DoneLabel->setForegroundRole(QPalette::Mid);
      } else {
        m_DoneLabel->setText(tr("Done"));
        m_DoneLabel->setForegroundRole(QPalette::WindowText);
      }
      if (m_Manager->isInfoIncomplete(downloadIndex)) {
        m_NameLabel->setText("<img src=\":/MO/gui/resources/dialog-warning_16.png\"/> " + m_NameLabel->text());
      }
    } else {
      m_DoneLabel->setVisible(false);
      m_Progress->setVisible(true);
      m_Progress->setValue(m_Manager->getProgress(downloadIndex));
    }

    painter->save();
    painter->translate(QPoint(0, option.rect.topLeft().y()));
    m_ItemWidget->render(painter);
    painter->restore();
  } catch (const std::exception &e) {
    qCritical("failed to paint download list item %d: %s", index.row(), e.what());
  }
}

QSize DownloadListWidgetCompactDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const
{
  const int width = m_ItemWidget->minimumWidth();
  const int height = m_ItemWidget->height();
  return QSize(width, height);
}


void DownloadListWidgetCompactDelegate::issueInstall()
{
  emit installDownload(m_ContextIndex.row());
}

void DownloadListWidgetCompactDelegate::issueQueryInfo()
{
  emit queryInfo(m_ContextIndex.row());
}

void DownloadListWidgetCompactDelegate::issueDelete()
{
  emit removeDownload(m_ContextIndex.row(), true);
}

void DownloadListWidgetCompactDelegate::issueRemoveFromView()
{
  emit removeDownload(m_ContextIndex.row(), false);
}

void DownloadListWidgetCompactDelegate::issueCancel()
{
  emit cancelDownload(m_ContextIndex.row());
}

void DownloadListWidgetCompactDelegate::issuePause()
{
  emit pauseDownload(m_ContextIndex.row());
}

void DownloadListWidgetCompactDelegate::issueResume()
{
  emit resumeDownload(m_ContextIndex.row());
}

void DownloadListWidgetCompactDelegate::issueDeleteAll()
{
  if (QMessageBox::question(NULL, tr("Are you sure?"),
                            tr("This will remove all finished downloads from this list and from disk."),
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
    emit removeDownload(-1, true);
  }
}

void DownloadListWidgetCompactDelegate::issueDeleteCompleted()
{
  if (QMessageBox::question(NULL, tr("Are you sure?"),
                            tr("This will remove all installed downloads from this list and from disk."),
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
    emit removeDownload(-2, true);
  }
}

void DownloadListWidgetCompactDelegate::issueRemoveFromViewAll()
{
  if (QMessageBox::question(NULL, tr("Are you sure?"),
                            tr("This will permanently remove all finished downloads from this list (but NOT from disk)."),
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
    emit removeDownload(-1, false);
  }
}

void DownloadListWidgetCompactDelegate::issueRemoveFromViewCompleted()
{
  if (QMessageBox::question(NULL, tr("Are you sure?"),
                            tr("This will permanently remove all installed downloads from this list (but NOT from disk)."),
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
    emit removeDownload(-2, false);
  }
}


bool DownloadListWidgetCompactDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                   const QStyleOptionViewItem &option, const QModelIndex &index)
{
  try {
    if (event->type() == QEvent::MouseButtonDblClick) {
      QModelIndex sourceIndex = qobject_cast<QSortFilterProxyModel*>(model)->mapToSource(index);
      if (m_Manager->getState(sourceIndex.row()) >= DownloadManager::STATE_READY) {
        emit installDownload(sourceIndex.row());
      } else if (m_Manager->getState(sourceIndex.row()) >= DownloadManager::STATE_PAUSED) {
        emit resumeDownload(sourceIndex.row());
      }
      return true;
    } else if (event->type() == QEvent::MouseButtonRelease) {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
      if (mouseEvent->button() == Qt::RightButton) {
        QMenu menu;
        m_ContextIndex = qobject_cast<QSortFilterProxyModel*>(model)->mapToSource(index);
        DownloadManager::DownloadState state = m_Manager->getState(m_ContextIndex.row());
        if (state >= DownloadManager::STATE_READY) {
          menu.addAction(tr("Install"), this, SLOT(issueInstall()));
          if (m_Manager->isInfoIncomplete(m_ContextIndex.row())) {
            menu.addAction(tr("Query Info"), this, SLOT(issueQueryInfo()));
          }
          menu.addAction(tr("Delete"), this, SLOT(issueDelete()));
          menu.addAction(tr("Remove from View"), this, SLOT(issueRemoveFromView()));
        } else if (state == DownloadManager::STATE_DOWNLOADING){
          menu.addAction(tr("Cancel"), this, SLOT(issueCancel()));
          menu.addAction(tr("Pause"), this, SLOT(issuePause()));
        } else if (state == DownloadManager::STATE_PAUSED) {
          menu.addAction(tr("Remove"), this, SLOT(issueDelete()));
          menu.addAction(tr("Resume"), this, SLOT(issueResume()));
        }

        menu.addSeparator();
        menu.addAction(tr("Delete Installed..."), this, SLOT(issueDeleteCompleted()));
        menu.addAction(tr("Delete All..."), this, SLOT(issueDeleteAll()));
        menu.addSeparator();
        menu.addAction(tr("Remove Installed..."), this, SLOT(issueRemoveFromViewCompleted()));
        menu.addAction(tr("Remove All..."), this, SLOT(issueRemoveFromViewAll()));
        menu.exec(mouseEvent->globalPos());

        event->accept();
        return false;
      }
    }
  } catch (const std::exception &e) {
    qCritical("failed to handle editor event: %s", e.what());
  }

  return QItemDelegate::editorEvent(event, model, option, index);
}

