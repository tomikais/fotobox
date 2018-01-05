/* preferences.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "preferences.h"
#include "ui_preferences.h"

#include <QDir>
#include <QDesktopWidget>


Preferences::Preferences(QWidget *parent) : QDialog(parent),
  m_ui(new Ui::Preferences),
  m_settings(QSettings::IniFormat, QSettings::UserScope, qApp->applicationName(), qApp->applicationName(), this)
{
  //setup UI
  m_ui->setupUi(this);

  //move to center
  setGeometry(QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));

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


auto Preferences::pictureDirectory() const -> QString
{
  QString picDir;
#if defined __APPLE__
  //macOS shit
  picDir = QApplication::applicationDirPath() + QDir::separator() +
    QStringLiteral("..") + QDir::separator() +
    QStringLiteral("..") + QDir::separator() +
    QStringLiteral("..") + QDir::separator();
#else
  picDir = QApplication::applicationDirPath() + QDir::separator();
#endif
  return picDir;
}
