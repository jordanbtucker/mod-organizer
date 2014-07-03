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

#include "simpleinstalldialog.h"
#include "ui_simpleinstalldialog.h"
#include <QLineEdit>

#include <QComboBox>

using namespace MOBase;


SimpleInstallDialog::SimpleInstallDialog(const GuessedValue<QString> &preset, QWidget *parent)
  : QDialog(parent), ui(new Ui::SimpleInstallDialog), m_Manual(false)
{
  ui->setupUi(this);

  for (auto iter = preset.variants().begin(); iter != preset.variants().end(); ++iter) {
    ui->nameCombo->addItem(*iter);
  }

  ui->nameCombo->setCurrentIndex(ui->nameCombo->findText(preset));

  setWindowFlags(windowFlags() & (~Qt::WindowContextHelpButtonHint));
  ui->nameCombo->setAutoCompletionCaseSensitivity(Qt::CaseSensitive);
}

SimpleInstallDialog::~SimpleInstallDialog()
{
    delete ui;
}

QString SimpleInstallDialog::getName() const
{
  return ui->nameCombo->currentText();
}

void SimpleInstallDialog::on_okBtn_clicked()
{
  this->accept();
}

void SimpleInstallDialog::on_cancelBtn_clicked()
{
  this->reject();
}

void SimpleInstallDialog::on_manualBtn_clicked()
{
  m_Manual = true;
  this->reject();
}
