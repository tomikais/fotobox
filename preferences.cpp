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

  //hide Preferences if they aren't available
  hidePreferences();

  //connect UI to preferences
  connect(m_ui->chbButtons, &QAbstractButton::toggled, this, &Preferences::setShowButtons);
  connect(m_ui->txtShowColor, &QLineEdit::textChanged, this, &Preferences::setBackgroundColor);
  connect(m_ui->txtShowColor, &QLineEdit::textChanged, m_ui->txtShowColor, &Preferences::setToolTip);
  connect(m_ui->txtShowColor, &QLineEdit::textChanged, this, &Preferences::showColor);
  connect(m_ui->spbInputPin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Preferences::setInputPin);
  connect(m_ui->spbOutputPin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Preferences::setOutputPin);
  connect(m_ui->spbQueryInterval, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Preferences::setQueryInterval);
  connect(m_ui->txtGphoto2Arg, &QLineEdit::textChanged, this, &Preferences::setArgumentLine);
  connect(m_ui->spbTimout, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &Preferences::setTimeoutValue);
  //connect buttons
  connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(this, &QDialog::accepted, this, &Preferences::savePreferences);
  connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(m_ui->buttonBox, &QDialogButtonBox::clicked, [&](QAbstractButton *button) {
    //identify restore button
    if (button == m_ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
      //restore defaults
      restoreDefaultPreferences();
    }
  });
  connect(m_ui->btnChooseColor, &QPushButton::clicked, this, &Preferences::colorDialog);

  //restore default values
  restoreDefaultPreferences();

  //load settings from ini file
  loadPreferences();

  //auto accept Dialog
  const int oneSecond = 1000;
  m_timer->setInterval(oneSecond);
  connect(m_timer, &QTimer::timeout, this, &Preferences::autoAcceptDialog);
  m_timer->start();
}


Preferences::~Preferences()
{
  delete m_ui;
}


void Preferences::autoAcceptDialog()
{
  if (m_counter >= 1) {
    //set Window Title and start timer again
    setWindowTitle(tr("launching FotoBox in ") + QString::number(m_counter) + tr(" seconds"));
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
  if (m_timer->isActive()) {
    m_timer->stop();
    setMouseTracking(false);
    setWindowTitle(tr("FotoBox preferences"));
  }

  //call base class method
  QWidget::mouseMoveEvent(event);
}


auto Preferences::loadPreferences() -> void
{
  //GENERAL
  m_ui->chbButtons->setChecked(m_settings.value(m_ui->chbButtons->objectName(), m_ui->chbButtons->isChecked()).toBool());
  m_ui->txtShowColor->setText(m_settings.value(m_ui->txtShowColor->objectName(), m_ui->txtShowColor->text()).toString());

  m_settings.beginGroup("Buzzer");
  m_ui->spbInputPin->setValue(m_settings.value(m_ui->spbInputPin->objectName(), m_ui->spbInputPin->value()).toInt());
  m_ui->spbOutputPin->setValue(m_settings.value(m_ui->spbOutputPin->objectName(), m_ui->spbOutputPin->value()).toInt());
  m_ui->spbQueryInterval->setValue(m_settings.value(m_ui->spbQueryInterval->objectName(), m_ui->spbQueryInterval->value()).toInt());
  m_settings.endGroup();

  m_settings.beginGroup("Camera");
  m_ui->txtGphoto2Arg->setText(m_settings.value(m_ui->txtGphoto2Arg->objectName(), m_ui->txtGphoto2Arg->text()).toString());
  m_ui->spbTimout->setValue(m_settings.value(m_ui->spbTimout->objectName(), m_ui->spbTimout->value()).toInt());
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
  m_ui->txtShowColor->setText(dialog.selectedColor().name());
}


auto Preferences::showColor(const QString i_colorName) -> void
{
  //create color
  QColor color(i_colorName);

  //set color (text + background)
  auto palette = m_ui->txtShowColor->palette();
  palette.setColor(QPalette::Text, color);
  palette.setColor(QPalette::Base, color);

  //apply styling
  m_ui->txtShowColor->setPalette(palette);
}


auto Preferences::savePreferences() -> void
{
  //GENERAL
  m_settings.setValue(m_ui->chbButtons->objectName(), showButtons());
  m_settings.setValue(m_ui->txtShowColor->objectName(), backgroundColor());

  m_settings.beginGroup("Buzzer");
  m_settings.setValue(m_ui->spbInputPin->objectName(), inputPin());
  m_settings.setValue(m_ui->spbOutputPin->objectName(), outputPin());
  m_settings.setValue(m_ui->spbQueryInterval->objectName(), queryInterval());
  m_settings.endGroup();

  m_settings.beginGroup("Camera");
  m_settings.setValue(m_ui->txtGphoto2Arg->objectName(), argumentLine());
  m_settings.setValue(m_ui->spbTimout->objectName(), timeoutValue());
  m_settings.endGroup();
}

auto Preferences::hidePreferences() -> void
{
  //hide buzzer settings if WiringPI isn't available
#if !defined __WIRING_PI_H__  && !defined QT_DEBUG 
  m_ui->lblBuzzer->hide();
  m_ui->lblInputPin->hide();
  m_ui->spbInputPin->hide();
  m_ui->lblOutputPin->hide();
  m_ui->spbOutputPin->hide();
  m_ui->lblQueryInterval->hide();
  m_ui->spbQueryInterval->hide();
#endif

  //hide camera settings if Platform is Windows
#if defined Q_OS_WIN && !defined QT_DEBUG 
  m_ui->lblCamera->hide();
  m_ui->lblGphoto2Arg->hide();
  m_ui->txtGphoto2Arg->hide();
  m_ui->lblTimeout->hide();
  m_ui->spbTimout->hide();
#endif

}

void Preferences::restoreDefaultPreferences()
{
  //General
  m_ui->chbButtons->setChecked(true);
  m_ui->txtShowColor->setText("#000000");

  //Buzzer
  m_ui->spbInputPin->setValue(5);
  m_ui->spbOutputPin->setValue(0);
  m_ui->spbQueryInterval->setValue(10);

  //Camera
  m_ui->txtGphoto2Arg->setText("--capture-image-and-download --keep --filename preview.jpg --set-config /main/settings/capturetarget=1 --force-overwrite");
  m_ui->spbTimout->setValue(15);
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


int Preferences::queryInterval()
{
  return m_queryInterval;
}

void Preferences::setQueryInterval(const int i_value)
{
  m_queryInterval = i_value;
  emit queryIntervalChanged(m_queryInterval);
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
