/* preferences.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "preferences.h"
#include "ui_preferences.h"

#include <QColorDialog>
#include <QDesktopWidget>
#include <QDir>


Preferences::Preferences(QWidget *parent) : QDialog(parent),
  m_ui(new Ui::Preferences),
  m_settings(QSettings::IniFormat, QSettings::UserScope, qApp->applicationName(), qApp->applicationName(), this)
{
  //setup UI
  m_ui->setupUi(this);

  //move to center
  setGeometry(QStyle::alignedRect( Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));

  //connect button
  connect(m_ui->btnStart, &QPushButton::clicked, this, &Preferences::close);
  connect(m_ui->btnQuit, &QPushButton::clicked, this, &Preferences::quitApplication);
  connect(m_ui->btnChooseColor, &QPushButton::clicked, this, &Preferences::colorDialog);
}


Preferences::~Preferences()
{
  delete m_ui;
}


auto Preferences::quitApplication() -> void
{
  //prevent to executing closeEvent
  done(EXIT_FAILURE);

  //Quit CoreApplication
  QMetaObject::invokeMethod(qApp, "quit", Qt::QueuedConnection);
}


auto Preferences::closeEvent(QCloseEvent *event) -> void 
{
  //GENERAL
  m_settings.setValue(m_ui->chbButtons->objectName(), m_ui->chbButtons->checkState());
  //m_settings.setValue(m_ui->lblShowColor->objectName(),
  
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

auto Preferences::colorDialog() -> void
{
  //"Color Picker" Dialog
  QColorDialog dialog(this);
  dialog.exec();

  //show the color which the user has selected
  m_ui->lblShowColor->setPalette(QPalette(dialog.selectedColor()));
  m_ui->lblShowColor->setText(dialog.selectedColor().name());
}
