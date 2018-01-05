/* preferences.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "preferences.h"
#include "ui_preferences.h"


Preferences::Preferences(QWidget *parent) : QDialog(parent),
  m_ui(new Ui::Preferences),
  m_settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName(), this)
{
  //setup UI
  m_ui->setupUi(this);

  //connect button
  connect(m_ui->btnOk, &QPushButton::clicked, this, &Preferences::close);
}


Preferences::~Preferences()
{
  delete m_ui;
}


auto Preferences::closeEvent(QCloseEvent *event) -> void 
{
  //GENERAL
  m_settings.setValue(m_ui->chbButtons->objectName(), m_ui->chbButtons->checkState());
  
  //BUZZER

  //CAMERA


  //default close request
  QDialog::closeEvent(event);
}