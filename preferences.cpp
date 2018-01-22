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
#include <QLabel>


Preferences::Preferences(QWidget *parent) : QDialog(parent),
m_ui(new Ui::Preferences),
m_settings(QSettings::IniFormat, QSettings::UserScope, qApp->applicationName(), qApp->applicationName(), this),
m_general{}
{
  //setup UI
  m_ui->setupUi(this);

  //move to center
  setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry()));

  //connect button
  connect(m_ui->btnStart, &QPushButton::clicked, this, &Preferences::close);
  connect(m_ui->btnQuit, &QPushButton::clicked, this, &Preferences::quitApplication);
  connect(m_ui->btnChooseColor, &QPushButton::clicked, this, &Preferences::colorDialog);

  //restore application settings
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
  m_general.showButtons = m_ui->chbButtons->checkState();
  m_settings.setValue(m_ui->chbButtons->objectName(), m_general.showButtons);
  m_general.backgroundColor = m_ui->lblShowColor->toolTip();
  m_settings.setValue(m_ui->lblShowColor->objectName(), m_general.backgroundColor);

  //BUZZER

  //CAMERA


  //default close event
  QDialog::closeEvent(event);
}


auto Preferences::showEvent(QShowEvent *event) -> void
{
  //GENERAL
  m_ui->chbButtons->setCheckState(static_cast<Qt::CheckState>(m_settings.value(m_ui->chbButtons->objectName(), m_ui->chbButtons->checkState()).toInt()));
  setLabelColor(m_ui->lblShowColor, m_settings.value(m_ui->lblShowColor->objectName(), m_ui->lblShowColor->toolTip()).toString());

  //BUZZER

  //CAMERA


  //default show event
  QDialog::showEvent(event);
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
  setLabelColor(m_ui->lblShowColor, dialog.selectedColor());
}


auto Preferences::setLabelColor(QLabel *i_label, const QColor& i_color) -> void
{
  //set color
  i_label->setPalette(i_color);
  //set tooltip
  i_label->setToolTip(i_color.name());
}
