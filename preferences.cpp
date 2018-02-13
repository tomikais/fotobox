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


auto Preferences::getInstance() -> Preferences&
{
  //thread safe static initializer
  static Preferences instance;
  return instance;
}


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
  connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(this, &QDialog::accepted, this, &Preferences::savePreferences);

  connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  connect(m_ui->btnChooseColor, &QPushButton::clicked, this, &Preferences::colorDialog);
}


Preferences::~Preferences()
{
  delete m_ui;
}


auto Preferences::savePreferences() -> void
{
  //GENERAL
  m_general.showButtons = m_ui->chbButtons->checkState();
  m_settings.setValue(m_ui->chbButtons->objectName(), m_general.showButtons);
  m_general.backgroundColor = m_ui->lblShowColor->toolTip();
  m_settings.setValue(m_ui->lblShowColor->objectName(), m_general.backgroundColor);

  //BUZZER
  m_buzzer.inputPin = m_ui->spbInputPin->value();
  m_settings.setValue(m_ui->spbInputPin->objectName(), m_buzzer.inputPin);
  m_buzzer.outputPin = m_ui->spbOutputPin->value();
  m_settings.setValue(m_ui->spbOutputPin->objectName(), m_buzzer.outputPin);

  //CAMERA
}


auto Preferences::showEvent(QShowEvent *event) -> void
{
  //GENERAL
  m_ui->chbButtons->setCheckState(static_cast<Qt::CheckState>(m_settings.value(m_ui->chbButtons->objectName(), m_ui->chbButtons->checkState()).toInt()));
  setLabelColor(m_ui->lblShowColor, m_settings.value(m_ui->lblShowColor->objectName(), m_ui->lblShowColor->toolTip()).toString());

  //BUZZER
  m_ui->spbInputPin->setValue(m_settings.value(m_ui->spbInputPin->objectName(), m_ui->spbInputPin->value()).toInt());
  m_ui->spbOutputPin->setValue(m_settings.value(m_ui->spbOutputPin->objectName(), m_ui->spbOutputPin->value()).toInt());

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
