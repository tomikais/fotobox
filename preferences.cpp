/* preferences.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "preferences.h"
#include "ui_preferences.h"

#include "preferenceprovider.h"
#include "fotobox.h"

#include <QColorDialog>
#include <QDesktopWidget>
#include <QProcess>
#include <QTimer>



Preferences::Preferences(QWidget *parent) : QDialog(parent),
  m_fotoBox(nullptr),
  m_ui(new Ui::PreferencesDialog),
  m_settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::applicationName(), QCoreApplication::applicationName(), this),
  m_timer(new QTimer(this))
{
  //setup UI
  m_ui->setupUi(this);

  //move to center
  setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), QApplication::desktop()->availableGeometry()));

  //hide Preferences if they aren't available
  hidePreferences();

  //connect UI to preferences
  connect(m_ui->txtPhotoFolder, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setPhotoFolder);
  connect(m_ui->txtPhotoName, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setPhotoName);
  connect(m_ui->chbButtons, &QAbstractButton::toggled, &PreferenceProvider::instance(), &PreferenceProvider::setShowButtons);
  connect(m_ui->txtShowColor, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setBackgroundColor);
  connect(m_ui->txtShowColor, &QLineEdit::textChanged, m_ui->txtShowColor, &QLineEdit::setToolTip);
  connect(m_ui->txtShowColor, &QLineEdit::textChanged, this, &Preferences::showColor);
  connect(m_ui->spbInputPin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setInputPin);
  connect(m_ui->spbOutputPin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setOutputPin);
  connect(m_ui->spbQueryInterval, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setQueryInterval);
  connect(m_ui->cmbCameraMode, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), &PreferenceProvider::instance(), &PreferenceProvider::setCameraMode);
  connect(m_ui->cmbCameraMode, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &Preferences::applicationAvailable);
  connect(m_ui->cmbCameraMode, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
          [&]() {
      //QComboBox has changed, show stored data to QLineEdit
      m_ui->txtArgumentLine->setText(m_ui->cmbCameraMode->currentData().toString());
    });
  connect(m_ui->txtArgumentLine, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setArgumentLine);
  connect(m_ui->txtArgumentLine, &QLineEdit::textChanged, this,
          [&](const QString& i_value) {
      //save changed text in QComboBox model
      m_ui->cmbCameraMode->setItemData(m_ui->cmbCameraMode->currentIndex(), i_value);
    });
  connect(m_ui->spbTimout, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setTimeoutValue);

  //connect buttons
  connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(this, &QDialog::accepted, this, &Preferences::savePreferences);
  connect(this, &QDialog::accepted, this, &Preferences::startFotoBox);
  connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(m_ui->buttonBox, &QDialogButtonBox::clicked, this,
          [&](QAbstractButton *button) {
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


void Preferences::startFotoBox()
{
  //Hide Preference dialog
  hide();

  m_timer->stop();
  setWindowTitle(tr("FotoBox preferences"));

  //Start FotoBox
  if (m_fotoBox) {
      //delete old one
      m_fotoBox->deleteLater();
    }
  m_fotoBox = new FotoBox(this);
  Q_CHECK_PTR(m_fotoBox);
  connect(m_fotoBox, &QDialog::rejected, this, &QApplication::restoreOverrideCursor);
  connect(m_fotoBox, &QDialog::rejected, this, &QDialog::show);
  m_fotoBox->showFullScreen();
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
      m_ui->scrollArea->setMouseTracking(false);
      m_ui->scrollAreaWidgetContents->setMouseTracking(false);
      setWindowTitle(tr("FotoBox preferences"));
    }

  //call base class method
  QWidget::mouseMoveEvent(event);
}


auto Preferences::loadPreferences() -> void
{
  m_settings.beginGroup(QStringLiteral("FotoBox"));
  m_ui->txtPhotoFolder->setText(m_settings.value(m_ui->txtPhotoFolder->objectName(), m_ui->txtPhotoFolder->text()).toString());
  m_ui->txtPhotoName->setText(m_settings.value(m_ui->txtPhotoName->objectName(), m_ui->txtPhotoName->text()).toString());
  m_ui->chbButtons->setChecked(m_settings.value(m_ui->chbButtons->objectName(), m_ui->chbButtons->isChecked()).toBool());
  m_ui->txtShowColor->setText(m_settings.value(m_ui->txtShowColor->objectName(), m_ui->txtShowColor->text()).toString());
  m_settings.endGroup();

  m_settings.beginGroup(QStringLiteral("Buzzer"));
  m_ui->spbInputPin->setValue(m_settings.value(m_ui->spbInputPin->objectName(), m_ui->spbInputPin->value()).toInt());
  m_ui->spbOutputPin->setValue(m_settings.value(m_ui->spbOutputPin->objectName(), m_ui->spbOutputPin->value()).toInt());
  m_ui->spbQueryInterval->setValue(m_settings.value(m_ui->spbQueryInterval->objectName(), m_ui->spbQueryInterval->value()).toInt());
  m_settings.endGroup();

  m_settings.beginGroup(QStringLiteral("Camera"));
  //restore QComboBox model
  auto data = m_settings.value(m_ui->cmbCameraMode->objectName() + QStringLiteral("Data")).toStringList();
  auto text = m_settings.value(m_ui->cmbCameraMode->objectName() + QStringLiteral("Text")).toStringList();
  if (!data.empty()) {
      m_ui->cmbCameraMode->clear();
      for (int i = 0; i < data.count(); ++i) {
          m_ui->cmbCameraMode->addItem(text.at(i), data.at(i));
        }
    }
  m_ui->cmbCameraMode->setCurrentText(m_settings.value(m_ui->cmbCameraMode->objectName(), m_ui->cmbCameraMode->currentText()).toString());

  m_ui->spbTimout->setValue(m_settings.value(m_ui->spbTimout->objectName(), m_ui->spbTimout->value()).toInt());
  m_settings.endGroup();
}


auto Preferences::colorDialog() -> void
{
  //"Color Picker" Dialog
  QColorDialog dialog(this);
  dialog.exec();

  //show the color which the user has selected
  m_ui->txtShowColor->setText(dialog.selectedColor().name());
}


auto Preferences::showColor(const QString& i_colorName) -> void
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
  m_settings.beginGroup(QStringLiteral("FotoBox"));
  m_settings.setValue(m_ui->txtPhotoFolder->objectName(), PreferenceProvider::instance().photoFolder());
  m_settings.setValue(m_ui->txtPhotoName->objectName(), PreferenceProvider::instance().photoName());
  m_settings.setValue(m_ui->chbButtons->objectName(), PreferenceProvider::instance().showButtons());
  m_settings.setValue(m_ui->txtShowColor->objectName(), PreferenceProvider::instance().backgroundColor());
  m_settings.endGroup();

  m_settings.beginGroup(QStringLiteral("Buzzer"));
  m_settings.setValue(m_ui->spbInputPin->objectName(), PreferenceProvider::instance().inputPin());
  m_settings.setValue(m_ui->spbOutputPin->objectName(), PreferenceProvider::instance().outputPin());
  m_settings.setValue(m_ui->spbQueryInterval->objectName(), PreferenceProvider::instance().queryInterval());
  m_settings.endGroup();

  m_settings.beginGroup(QStringLiteral("Camera"));
  //Save QComboBox model
  QStringList text, data;
  for (int i = 0; i < m_ui->cmbCameraMode->count(); ++i) {
      text << m_ui->cmbCameraMode->itemText(i);
      data << m_ui->cmbCameraMode->itemData(i).toString();
    }
  m_settings.setValue(m_ui->cmbCameraMode->objectName() + QStringLiteral("Text"), text);
  m_settings.setValue(m_ui->cmbCameraMode->objectName() + QStringLiteral("Data"), data);
  m_settings.setValue(m_ui->cmbCameraMode->objectName(), PreferenceProvider::instance().cameraMode());

  m_settings.setValue(m_ui->spbTimout->objectName(), PreferenceProvider::instance().timeoutValue());
  m_settings.endGroup();
}


auto Preferences::hidePreferences() -> void
{
  //hide buzzer settings if WiringPI isn't available
#if defined __WIRING_PI_H__  && !defined QT_DEBUG
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


auto Preferences::restoreDefaultPreferences() -> void
{
  //FotoBox
#if defined Q_OS_MACOS
  m_ui->txtPhotoFolder->setText(QStringLiteral("~/Pictures/FotoBox/"));
#endif
#if defined Q_OS_LINUX
  m_ui->txtPhotoFolder->setText(QStringLiteral("~/FotoBox/"));
#endif
#if defined Q_OS_WIN
  m_ui->txtPhotoFolder->setText(QStringLiteral("%CSIDL_DEFAULT_MYPICTURES%\\FotoBox\\"));
#endif
  m_ui->txtPhotoName->setText(QStringLiteral("eventname.jpg"));
  m_ui->chbButtons->setChecked(false);
  m_ui->txtShowColor->setText(QStringLiteral("#000000"));

  //Buzzer
  m_ui->spbInputPin->setValue(5);
  m_ui->spbOutputPin->setValue(0);
  m_ui->spbQueryInterval->setValue(10);

  //Camera
  m_ui->cmbCameraMode->clear();
  m_ui->cmbCameraMode->addItem(
        QStringLiteral("gphoto2"),
        QStringLiteral("--capture-image-and-download --keep --filename %1 --set-config /main/settings/capturetarget=1 --force-overwrite"));
  m_ui->cmbCameraMode->addItem(
        QStringLiteral("raspistill"),
        QStringLiteral("--output \"%1\" --width 1920 --height 1080 --quality 75 --nopreview --timeout 1"));
  m_ui->spbTimout->setValue(30);
}


auto Preferences::applicationAvailable(const QString& i_name) -> void
{
  if (i_name == QLatin1String("gphoto2")) {
      auto process = new QProcess(this);
      //specific 'gphoto2' check: auto-detect: get detected cameras
      process->start(i_name, { QStringLiteral("--auto-detect"), QStringLiteral("--version") });
      if (process->waitForFinished() && process->exitCode() != EXIT_SUCCESS) {
          m_ui->lblCameraModeInfo->setText(tr("'%1' is missing! Get it ").arg(i_name) + QStringLiteral("<a href='https://github.com/gonzalo/gphoto2-updater'>Linux (gphoto2 updater)</a> / <a href='https://brew.sh/'>macOS (Homebrew)</a>"));
        } else {
          auto output = process->readAllStandardOutput();
          //gphoto version
          auto version = output.left(output.indexOf('\n'));
          //get camera model
          auto model = output.right(output.size() - output.lastIndexOf('-') - 2);
          model = model.left(model.indexOf(')') + 1 );
          m_ui->lblCameraModeInfo->setText(version + QStringLiteral(" / ") + model);
        }
      process->deleteLater();
      return;
    }
  if (i_name == QStringLiteral("raspistill")) {
      if (QProcess::execute(i_name, { QStringLiteral("--help") }) != EXIT_SUCCESS) {
          //specific 'raspistill' show verbose message
          m_ui->lblCameraModeInfo->setText(tr("'%1' is missing! Get it ").arg(i_name) + QStringLiteral("<a href='https://www.raspberrypi.org/documentation/usage/camera/README.md'>Raspberry Pi (connecting and enabling the camera)</a>"));
        }
      return;
    }
  if (!i_name.isEmpty()) {
      if (QProcess::execute(i_name) != EXIT_SUCCESS) {
          //other applications
          m_ui->lblCameraModeInfo->setText(QStringLiteral("'") + i_name + tr("' is missing!"));
        }
      return;

  m_ui->lblCameraModeInfo->setText(QString());
}
