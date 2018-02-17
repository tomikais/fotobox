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
#include <QTimer>


auto Preferences::getInstance() -> Preferences&
{
  //thread safe static initializer
  static Preferences instance;
  return instance;
}


Preferences::Preferences(QWidget *parent) : QDialog(parent),
  m_ui(new Ui::Preferences),
  m_settings(QSettings::IniFormat, QSettings::UserScope, qApp->applicationName(), qApp->applicationName(), this),
  m_timer(new QTimer(this))
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

  //connect preferences to UI
  connect(this, &Preferences::showButtonsChanged, m_ui->chbButtons, &QAbstractButton::setChecked);
  connect(this, &Preferences::backgroundColorChanged, m_ui->txtShowColor, &QLineEdit::setText);
  connect(this, &Preferences::inputPinChanged, m_ui->spbInputPin, &QSpinBox::setValue);
  connect(this, &Preferences::outputPinChanged, m_ui->spbOutputPin, &QSpinBox::setValue);
  connect(this, &Preferences::argumentLineChanged, m_ui->txtGphoto2Arg, &QLineEdit::setText);
  connect(this, &Preferences::timeoutValueChanged, m_ui->spbTimout, &QSpinBox::setValue);

  //load settings from ini file
  initializePreferences();

  //connect UI to preferences
  connect(m_ui->chbButtons, &QAbstractButton::toggled, this, &Preferences::setShowButtons);
  connect(m_ui->txtShowColor, &QLineEdit::textChanged, this, &Preferences::setBackgroundColor);
  connect(m_ui->spbInputPin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Preferences::setInputPin);
  connect(m_ui->spbOutputPin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Preferences::setOutputPin);
  connect(m_ui->txtGphoto2Arg, &QLineEdit::textChanged, this, &Preferences::setArgumentLine);
  connect(m_ui->spbTimout, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Preferences::setTimeoutValue);

  //auto accept Dialog
  const int oneSecond = 1000;
  m_timer->setInterval(oneSecond);
  connect(m_timer, &QTimer::timeout, this, &Preferences::autoAcceptDialog);
  setMouseTracking(true);
  m_timer->start();
}


Preferences::~Preferences()
{
  delete m_ui;
}


void Preferences::autoAcceptDialog()
{
  if(m_counter >= 1) {
      //set Window Title and start timer again
      setWindowTitle(tr("launching Fotobox in ") + QString::number(m_counter) + tr(" seconds"));
      --m_counter;
      m_timer->start();
      return;
    }

  //stop timer and close dialog
  m_timer->stop();
  emit accept();
}


auto Preferences::mouseMoveEvent(QMouseEvent *event) -> void
{
  if(m_timer->isActive()) {
      m_timer->stop();
      setMouseTracking(false);
      setWindowTitle(tr("Fotobox preferences"));
    }

  //call base class method
  QWidget::mouseMoveEvent(event);
}


auto Preferences::initializePreferences() -> void
{
  //GENERAL
  setShowButtons(m_settings.value(m_ui->chbButtons->objectName(), m_ui->chbButtons->isChecked()).toBool());
  setLabelColor(m_ui->txtShowColor, m_settings.value(m_ui->txtShowColor->objectName(), m_ui->txtShowColor->text()).toString());

  m_settings.beginGroup("Buzzer");
  setInputPin(m_settings.value(m_ui->spbInputPin->objectName(), m_ui->spbInputPin->value()).toInt());
  setOutputPin(m_settings.value(m_ui->spbOutputPin->objectName(), m_ui->spbOutputPin->value()).toInt());
  m_settings.endGroup();

  m_settings.beginGroup("Camera");
  setArgumentLine(m_settings.value(m_ui->txtGphoto2Arg->objectName(), m_ui->txtGphoto2Arg->text()).toString());
  setTimeoutValue(m_settings.value(m_ui->spbTimout->objectName(), m_ui->spbTimout->value()).toInt());
  m_settings.endGroup();
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
  setLabelColor(m_ui->txtShowColor, dialog.selectedColor());
}


auto Preferences::setLabelColor(QLineEdit *i_lineEdit, const QColor& i_color) -> void
{
  //set color (text + background)
  QPalette palette;
  palette.setColor(QPalette::Text, i_color);
  palette.setColor(QPalette::Base, i_color);
  i_lineEdit->setPalette(palette);

  //set text and tooltip
  setBackgroundColor(i_color.name());
  i_lineEdit->setToolTip(i_color.name());
}


auto Preferences::savePreferences() -> void
{
  //GENERAL
  m_settings.setValue(m_ui->chbButtons->objectName(), showButtons());
  m_settings.setValue(m_ui->txtShowColor->objectName(), backgroundColor());

  m_settings.beginGroup("Buzzer");
  m_settings.setValue(m_ui->spbInputPin->objectName(), inputPin());
  m_settings.setValue(m_ui->spbOutputPin->objectName(), outputPin());
  m_settings.endGroup();

  m_settings.beginGroup("Camera");
  m_settings.setValue(m_ui->txtGphoto2Arg->objectName(), argumentLine());
  m_settings.setValue(m_ui->spbTimout->objectName(), timeoutValue());
  m_settings.endGroup();
}


//Qt Porperty
bool Preferences::showButtons()
{
  return m_showButtons;
}

void Preferences::setShowButtons(const bool i_value)
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
  m_outputPin = i_value;
  emit outputPinChanged(m_outputPin);
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
