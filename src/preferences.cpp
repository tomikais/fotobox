/* preferences.cpp
 *
 * Copyright (c) 2018 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "fotobox.h"
#include "preferenceprovider.h"
#include "preferences.h"
#include "ui_preferences.h"

#if !defined (Q_OS_WIN)
#include <sysexits.h>
#else
#define EX_USAGE 64 /* Windows: command line usage error */
#endif

#include <QColorDialog>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QProcess>
#include <QTimer>


Preferences::Preferences(QWidget *parent) : QDialog(parent),
  m_ui(new Ui::PreferencesDialog),
  m_settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::applicationName(), QCoreApplication::applicationName(), this),
  m_timer(new QTimer(this))
{
  //setup UI
  m_ui->setupUi(this);

  //move to center
  setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), QApplication::desktop()->availableGeometry()));

  //delete everything on close
  setAttribute(Qt::WA_DeleteOnClose);

  //connect UI to preferences
  connect(m_ui->txtPhotoFolder, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setPhotoFolder);
  connect(m_ui->btnChooseDirectory, &QPushButton::clicked, this, &Preferences::chooseDirectory);
  connect(m_ui->txtPhotoName, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setPhotoName);
  connect(m_ui->spbCountdown, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setCountdown);
  connect(m_ui->btnChooseColorCD, &QPushButton::clicked, this, &Preferences::colorDialog);
  connect(m_ui->txtShowColorCD, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setCountdownColor);
  connect(m_ui->txtShowColorCD, &QLineEdit::textChanged, m_ui->txtShowColorCD, &QLineEdit::setToolTip);
  connect(m_ui->txtShowColorCD, &QLineEdit::textChanged, this, &Preferences::showColor);
  connect(m_ui->chbButtons, &QAbstractButton::toggled, &PreferenceProvider::instance(), &PreferenceProvider::setShowButtons);
  connect(m_ui->btnChooseColorBG, &QPushButton::clicked, this, &Preferences::colorDialog);
  connect(m_ui->txtShowColorBG, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setBackgroundColor);
  connect(m_ui->txtShowColorBG, &QLineEdit::textChanged, m_ui->txtShowColorBG, &QLineEdit::setToolTip);
  connect(m_ui->txtShowColorBG, &QLineEdit::textChanged, this, &Preferences::showColor);
  connect(m_ui->spbInputPin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setInputPin);
  connect(m_ui->spbOutputPin, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setOutputPin);
  connect(m_ui->spbQueryInterval, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setQueryInterval);
  connect(m_ui->cmbCameraMode, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), &PreferenceProvider::instance(), &PreferenceProvider::setCameraMode);
  connect(m_ui->cmbCameraMode, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged), this, &Preferences::applicationAvailable);
  connect(m_ui->cmbCameraMode, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [&]() {
      //QComboBox has changed, show stored data to QLineEdit
      m_ui->txtArgumentLine->setText(m_ui->cmbCameraMode->currentData().toString());
    });
  connect(m_ui->txtArgumentLine, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setArgumentLine);
  connect(m_ui->txtArgumentLine, &QLineEdit::textChanged, this, [&](const QString& i_value) {
      //save changed text in QComboBox model
      m_ui->cmbCameraMode->setItemData(m_ui->cmbCameraMode->currentIndex(), i_value);
    });
  connect(m_ui->spbTimout, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setTimeoutValue);

  //connect buttons
  connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &Preferences::startFotoBox);
  connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(m_ui->buttonBox, &QDialogButtonBox::clicked, this, [&](QAbstractButton *button) {
    //identify restore button and restore defaults
    if (button == m_ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
        restoreDefaultPreferences();
      }
  });

  //restore default values
  restoreDefaultPreferences();

  //load settings from ini file
  loadPreferences();

  //auto accept Dialog
  m_timer->setInterval(PreferenceProvider::ONE_SECOND);
  connect(m_timer, &QTimer::timeout, this, &Preferences::autoAcceptDialog);
  m_timer->start();
}


void Preferences::startFotoBox()
{
  //save settings to ini file
  savePreferences();
  m_timer->stop();

  //Start FotoBox;
  auto *dialog = new FotoBox;

  //close dialog and start fotobox
  reject();
  dialog->showFullScreen();
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

  //start FotoBox
  startFotoBox();
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
  m_ui->spbCountdown->setValue(m_settings.value(m_ui->spbCountdown->objectName(), m_ui->spbCountdown->value()).toInt());
  m_ui->txtShowColorCD->setText(m_settings.value(m_ui->txtShowColorCD->objectName(), m_ui->txtShowColorCD->text()).toString());
  m_ui->chbButtons->setChecked(m_settings.value(m_ui->chbButtons->objectName(), m_ui->chbButtons->isChecked()).toBool());
  m_ui->txtShowColorBG->setText(m_settings.value(m_ui->txtShowColorBG->objectName(), m_ui->txtShowColorBG->text()).toString());
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

  if (dialog.exec() == QDialog::Accepted) {
      //show the color which the user has selected
      auto* button = qobject_cast<QPushButton*>(sender());
      if (button == m_ui->btnChooseColorCD) {
          //font countdown
          m_ui->txtShowColorCD->setText(dialog.selectedColor().name());
        }
      if (button == m_ui->btnChooseColorBG) {
          //background color
          m_ui->txtShowColorBG->setText(dialog.selectedColor().name());
        }
    }
}


auto Preferences::chooseDirectory() -> void
{
  //File Dialog to choose photo folder
  QFileDialog dialog(this, tr("choose directory"), QStringLiteral("/home"));
  dialog.setOptions(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

  //only set it if user don't abort dialog
  if (dialog.exec() == QDialog::Accepted) {
      auto path = dialog.directory().absolutePath();
      m_ui->txtPhotoFolder->setText(QDir::toNativeSeparators(path));
    }
}


auto Preferences::showColor(const QString& i_colorName) -> void
{
  //create color
  QColor color(i_colorName);

  //set color (text + background)
  auto* lineEdit = qobject_cast<QLineEdit*>(sender());
  if (lineEdit != nullptr) {
      auto palette = lineEdit->palette();
      palette.setColor(QPalette::Text, color);
      palette.setColor(QPalette::Base, color);
      lineEdit->setPalette(palette);
      //force repaint (Restore Default issue)
      lineEdit->repaint();
    }
}


auto Preferences::savePreferences() -> void
{
  m_settings.beginGroup(QStringLiteral("FotoBox"));
  m_settings.setValue(m_ui->txtPhotoFolder->objectName(), PreferenceProvider::instance().photoFolder());
  m_settings.setValue(m_ui->txtPhotoName->objectName(), PreferenceProvider::instance().photoName());
  m_settings.setValue(m_ui->spbCountdown->objectName(), PreferenceProvider::instance().countdown());
  m_settings.setValue(m_ui->txtShowColorCD->objectName(), PreferenceProvider::instance().countdownColor());
  m_settings.setValue(m_ui->chbButtons->objectName(), PreferenceProvider::instance().showButtons());
  m_settings.setValue(m_ui->txtShowColorBG->objectName(), PreferenceProvider::instance().backgroundColor());
  m_settings.endGroup();

  m_settings.beginGroup(QStringLiteral("Buzzer"));
  m_settings.setValue(m_ui->spbInputPin->objectName(), PreferenceProvider::instance().inputPin());
  m_settings.setValue(m_ui->spbOutputPin->objectName(), PreferenceProvider::instance().outputPin());
  m_settings.setValue(m_ui->spbQueryInterval->objectName(), PreferenceProvider::instance().queryInterval());
  m_settings.endGroup();

  m_settings.beginGroup(QStringLiteral("Camera"));
  //Save QComboBox model
  QStringList itemText, itemData;
  for (int i = 0; i < m_ui->cmbCameraMode->count(); ++i) {
      itemText << m_ui->cmbCameraMode->itemText(i);
      itemData << m_ui->cmbCameraMode->itemData(i).toString();
    }
  m_settings.setValue(m_ui->cmbCameraMode->objectName() + QStringLiteral("Text"), itemText);
  m_settings.setValue(m_ui->cmbCameraMode->objectName() + QStringLiteral("Data"), itemData);
  m_settings.setValue(m_ui->cmbCameraMode->objectName(), PreferenceProvider::instance().cameraMode());

  m_settings.setValue(m_ui->spbTimout->objectName(), PreferenceProvider::instance().timeoutValue());
  m_settings.endGroup();
}


auto Preferences::restoreDefaultPreferences() -> void
{
  //FotoBox
  m_ui->txtPhotoFolder->setText(QDir::toNativeSeparators(QCoreApplication::applicationDirPath()));
  m_ui->txtPhotoName->setText(QStringLiteral("eventname.jpg"));
  m_ui->spbCountdown->setValue(3);
  m_ui->txtShowColorCD->setText(QStringLiteral("#ff0000"));
#if !defined (Q_OS_UNIX)
  m_ui->chbButtons->setChecked(false);
#else
  m_ui->chbButtons->setChecked(true);
#endif
  m_ui->txtShowColorBG->setText(QStringLiteral("#000000"));

  //Buzzer
  m_ui->spbInputPin->setValue(5);
  m_ui->spbOutputPin->setValue(0);
  m_ui->spbQueryInterval->setValue(10);

  //Camera
  m_ui->cmbCameraMode->clear();
  m_ui->cmbCameraMode->addItem(
        QStringLiteral("gphoto2"),
        QLatin1String("--capture-image-and-download --keep --filename %1 --set-config /main/settings/capturetarget=1 --force-overwrite"));
  m_ui->cmbCameraMode->addItem(
        QStringLiteral("raspistill"),
        QLatin1String("--output %1 --width 1920 --height 1080 --quality 75 --nopreview --timeout 1"));
  m_ui->spbTimout->setValue(30);
}


auto Preferences::applicationAvailable(const QString& i_name) -> void
{
  m_ui->lblCameraModeInfo->setStyleSheet(QStringLiteral(""));
  if (i_name == QLatin1String("gphoto2")) {
      auto process = new QProcess(this);
      //specific 'gphoto2' check: auto-detect: get detected cameras
#if defined (Q_OS_WIN)
      //try use Windows 10 Linux Subsystem to call gphoto2
      process->start(QLatin1String("bash.exe -c '") + i_name + QLatin1String(" --auto-detect --version '"));
#else
      process->start(i_name, { QStringLiteral("--auto-detect"), QStringLiteral("--version") });
#endif
      if (process->waitForFinished() && process->exitCode() != EXIT_SUCCESS) {
          m_ui->lblCameraModeInfo->setStyleSheet(QStringLiteral("QLabel { color : red; }"));
          m_ui->lblCameraModeInfo->setText(tr("'%1' is missing! Get it ").arg(i_name) + QStringLiteral("<a href='https://github.com/gonzalo/gphoto2-updater'>Linux (gphoto2 updater)</a> / <a href='https://brew.sh/'>macOS (Homebrew)</a>"));
        } else {
          auto output = process->readAllStandardOutput();
          //gphoto version
          auto version = output.left(output.indexOf('\n'));
          //get camera model
          QString model = output.right(output.size() - output.lastIndexOf('-') - 2);
          model = model.left(model.indexOf(QLatin1String("usb"), Qt::CaseInsensitive));
          if (model.isEmpty()) {
              m_ui->lblCameraModeInfo->setStyleSheet(QStringLiteral("QLabel { color : red; }"));
              model = tr("*** no camera detected ***");
            }
          m_ui->lblCameraModeInfo->setText(version + QStringLiteral(" / ") + model.trimmed());
        }
      process->deleteLater();
      return;
    }
  if (i_name == QLatin1String("raspistill")) {
      if (QProcess::execute(i_name, { QStringLiteral("--help") }) != EX_USAGE) {
          //specific 'raspistill' show verbose message
          m_ui->lblCameraModeInfo->setStyleSheet(QStringLiteral("QLabel { color : red; }"));
          m_ui->lblCameraModeInfo->setText(tr("'%1' is missing! Get it ").arg(i_name) + QStringLiteral("<a href='https://www.raspberrypi.org/documentation/usage/camera/README.md'>Raspberry Pi (connecting and enabling the camera)</a>"));
        } else {
          m_ui->lblCameraModeInfo->clear();
        }
      return;
    }
  if (!i_name.isEmpty()) {
      if (QProcess::execute(i_name) != EXIT_SUCCESS) {
          //other applications
          m_ui->lblCameraModeInfo->setStyleSheet(QStringLiteral("QLabel { color : red; }"));
          m_ui->lblCameraModeInfo->setText(QStringLiteral("'") + i_name + tr("' is missing!"));
        } else {
          m_ui->lblCameraModeInfo->clear();
        }
      return;
    }
  m_ui->lblCameraModeInfo->clear();
}
