#include "handlerwindow.h"
#include "ui_handlerwindow.h"
#include "addbinarydialog.h"
#include <QMenu>
#include <QMessageBox>
#include <QDir>


HandlerWindow::HandlerWindow(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::HandlerWindow)
{
  ui->setupUi(this);


  connect(ui->actionAdd, SIGNAL(triggered()), this, SLOT(addBinaryDialog()));
  connect(ui->actionRemove, SIGNAL(triggered()), this, SLOT(removeBinary()));
}

HandlerWindow::~HandlerWindow()
{
  delete ui;
}

void HandlerWindow::setPrimaryHandler(const QString &handlerPath)
{
  if (handlerPath == QCoreApplication::applicationFilePath()) {
    ui->registerButton->setEnabled(false);
    ui->handlerView->setText(tr("<Current>"));
  } else {
    ui->handlerView->setText(handlerPath);
  }
}


void HandlerWindow::setHandlerStorage(HandlerStorage *storage)
{
  m_Storage = storage;

  ui->handlersWidget->clear();
  auto list = storage->handlers();
  for (auto iter = list.begin(); iter != list.end(); ++iter) {
    QTreeWidgetItem *newItem = new QTreeWidgetItem(QStringList() << iter->games.join(",") << QDir::toNativeSeparators(iter->executable));

    newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
    ui->handlersWidget->addTopLevelItem(newItem);
  }
}

void HandlerWindow::closeEvent(QCloseEvent *event)
{
  m_Storage->clear();
  for (int i = 0; i < ui->handlersWidget->topLevelItemCount(); ++i) {
    QTreeWidgetItem *item = ui->handlersWidget->topLevelItem(i);
    m_Storage->registerHandler(item->text(0).split(","), item->text(1), false, false);
  }
  QMainWindow::closeEvent(event);
}

void HandlerWindow::addBinaryDialog()
{
  AddBinaryDialog dialog(m_Storage->knownGames());
  if (dialog.exec() == QDialog::Accepted) {
    bool executableKnown = false;
    for (int i = 0; i < ui->handlersWidget->topLevelItemCount(); ++i) {
      QTreeWidgetItem *iterItem = ui->handlersWidget->topLevelItem(i);
      if (QFileInfo(iterItem->text(1)) == QFileInfo(dialog.executable())) {
        QStringList games = iterItem->text(0).split(",");
        games.append(dialog.gameIDs());
        games = games.toSet().toList();
        iterItem->setText(0, games.join(","));
        executableKnown = true;
      }
    }

    if (!executableKnown) {
      QTreeWidgetItem *newItem = new QTreeWidgetItem(QStringList() << dialog.gameIDs().join(",") << dialog.executable());
      newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
      ui->handlersWidget->insertTopLevelItem(0, newItem);
    }
  }
}

void HandlerWindow::removeBinary()
{
  ui->handlersWidget->takeTopLevelItem(m_ContextIndex.row());
}

void HandlerWindow::on_handlersWidget_customContextMenuRequested(const QPoint &pos)
{
  QMenu contextMenu;

  m_ContextIndex = ui->handlersWidget->indexAt(pos);
  if (m_ContextIndex.isValid()) {
    contextMenu.addAction(ui->actionRemove);
  } else {
    contextMenu.addAction(ui->actionAdd);
  }

  contextMenu.move(ui->handlersWidget->mapToGlobal(pos));
  contextMenu.exec();
}

void HandlerWindow::on_registerButton_clicked()
{
  if (QMessageBox::question(this, tr("Change handler registration?"),
                            tr("This will make the nxmhandler.exe you called the primary handler registered in the system.\n"
                               "That has no immediate impact on how links are handled.\nUse this if you moved Mod Organizer "
                               "or if you uninstalled the Mod Organizer installation that was previously registered. Continue?"),
                            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
    ui->handlerView->setText(tr("<Current>"));
    ui->registerButton->setEnabled(false);

    m_Storage->registerProxy(QCoreApplication::applicationFilePath());
  }
}
