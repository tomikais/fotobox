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

auto Preferences::getInstance() -> Preferences&
{
  //thread safe static initializer
  static Preferences instance;
  return instance;
}


Preferences::Preferences(QWidget *parent) : QDialog(parent),
  m_ui(new Ui::Preferences),
  m_settings(QSettings::IniFormat, QSettings::UserScope, qApp->applicationName(), qApp->applicationName(), this)
{
  //setup UI
  m_ui->setupUi(this);

  //load settings from ini file
  loadPreferences();

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
  setShowButtons(m_ui->chbButtons->checkState());
  m_settings.setValue(m_ui->chbButtons->objectName(), showButtons());
  setBackgroundColor(m_ui->lblShowColor->toolTip());
  m_settings.setValue(m_ui->lblShowColor->objectName(), backgroundColor());

  //BUZZER
  m_settings.beginGroup("Buzzer");
  setInputPin(m_ui->spbInputPin->value());
  m_settings.setValue(m_ui->spbInputPin->objectName(), inputPin());
  setOutputPin(m_ui->spbOutputPin->value());
  m_settings.setValue(m_ui->spbOutputPin->objectName(), outputPin());
  m_settings.endGroup();

  //CAMERA
  m_settings.beginGroup("Camera");
  setArgumentLine(m_ui->txtGphoto2Arg->text());
  m_settings.setValue(m_ui->txtGphoto2Arg->objectName(), argumentLine());
  setTimeoutValue(m_ui->spbTimout->value());
  m_settings.setValue(m_ui->spbTimout->objectName(), timeoutValue());
  m_settings.endGroup();
}


auto Preferences::loadPreferences() -> void
{
  //GENERAL
  m_ui->chbButtons->setCheckState(static_cast<Qt::CheckState>(m_settings.value(m_ui->chbButtons->objectName(), m_ui->chbButtons->checkState()).toInt()));
  setLabelColor(m_ui->lblShowColor, m_settings.value(m_ui->lblShowColor->objectName(), m_ui->lblShowColor->toolTip()).toString());

  //BUZZER
  m_ui->spbInputPin->setValue(m_settings.value(m_ui->spbInputPin->objectName(), m_ui->spbInputPin->value()).toInt());
  m_ui->spbOutputPin->setValue(m_settings.value(m_ui->spbOutputPin->objectName(), m_ui->spbOutputPin->value()).toInt());

  //CAMERA
m_ui->txtGphoto2Arg->setText(m_settings.value(m_ui->txtGphoto2Arg->objectName(), m_ui->txtGphoto2Arg->text()).toString());
  m_ui->spbTimout->setValue(m_settings.value(m_ui->spbTimout->objectName(), m_ui->spbTimout->value()).toInt());
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


//Qt Porperty
Qt::CheckState Preferences::showButtons()
{
  return m_showButtons;
}

void Preferences::setShowButtons(const Qt::CheckState i_value)
{
  m_showButtons = i_value;
  emit showButtonsChanged(m_showButtons);
}

QString& Preferences::backgroundColor()
{
  return m_backgroundColor;
}

void Preferences::setBackgroundColor(const QString& i_value)
{
  m_backgroundColor = i_value;
  emit backgroundColorChanged(m_backgroundColor);
}


int Preferences::inputPin()
{
  return m_inputPin;
}

void Preferences::setInputPin(const int i_value)
{
  m_inputPin = i_value;
  emit inputPinChanged(m_inputPin);
}


int Preferences::outputPin()
{
  return m_outputPin;
}

void Preferences::setOutputPin(const int i_value)
{
  m_inputPin = i_value;
  emit outputPinChanged(m_inputPin);
}


QString Preferences::argumentLine()
{
  return m_argumentLine;
}

void Preferences::setArgumentLine(const QString& i_value)
{
  m_argumentLine = i_value;
  emit argumentLineChanged(m_argumentLine);
}


int Preferences::timeoutValue()
{
  return m_timeoutValue;
}

void Preferences::setTimeoutValue(const int i_value)
{
  m_timeoutValue = i_value;
  emit timeoutValueChanged(m_timeoutValue);
}
