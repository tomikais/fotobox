/* preferences.cpp
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#include "fotobox.h"

#include "preferenceprovider.h"
#include "preferences.h"
#include "ui_preferences.h"

#include <QColorDialog>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QProcess>
#include <QScreen>

Preferences::Preferences(QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::PreferencesDialog)
    , m_settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::applicationName(), QCoreApplication::applicationName(), this)
    , m_countdown(this, 10)
{
    //setup UI
    m_ui->setupUi(this);

    //move to center
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), QGuiApplication::primaryScreen()->availableGeometry()));

    //delete everything on close
    setAttribute(Qt::WA_DeleteOnClose);

    //Signal & Slot
    connectUi();

    //restore default values
    restoreDefaultPreferences();

    //load settings from ini file
    loadPreferences();

    //auto accept Dialog
    connect(&m_countdown, &Countdown::elapsed, this, [&]() {
        //start FotoBox
        startFotoBox();
    });
    m_countdown.start();

    //update window title
    connect(&m_countdown, &Countdown::update, this, [&](unsigned int i_timeLeft) {
        //: %1 countdown (number)
        setWindowTitle(tr("launching FotoBox in %1 seconds").arg(i_timeLeft));
    });

    //function only available Qt 5.5 or newer
#if (QT_VERSION < QT_VERSION_CHECK(5, 5, 0))
    m_ui->chbGrayscale->setEnabled(false);
#endif
}

void Preferences::connectUi()
{
    //connect UI to preferences
    connect(m_ui->txtPhotoFolder, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setPhotoFolder);
    connect(m_ui->btnChooseDirectory, &QPushButton::clicked, this, &Preferences::chooseDirectory);
    connect(m_ui->txtPhotoName, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setPhotoName);
    connect(m_ui->spbCountdown, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setCountdown);
    connect(m_ui->btnChooseColorCD, &QPushButton::clicked, this, &Preferences::colorDialog);
    connect(m_ui->txtShowColorCD, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setCountdownColor);
    connect(m_ui->txtShowColorCD, &QLineEdit::textChanged, m_ui->txtShowColorCD, &QLineEdit::setToolTip);
    connect(m_ui->txtShowColorCD, &QLineEdit::textChanged, this, &Preferences::showColor);
    connect(m_ui->chbButtons, &QAbstractButton::toggled, &PreferenceProvider::instance(), &PreferenceProvider::setShowButtons);
    connect(m_ui->btnChooseColorBG, &QPushButton::clicked, this, &Preferences::colorDialog);
    connect(m_ui->txtShowColorBG, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setBackgroundColor);
    connect(m_ui->txtShowColorBG, &QLineEdit::textChanged, m_ui->txtShowColorBG, &QLineEdit::setToolTip);
    connect(m_ui->txtShowColorBG, &QLineEdit::textChanged, this, &Preferences::showColor);
    connect(m_ui->spbInputPin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setInputPin);
    connect(m_ui->spbOutputPin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setOutputPin);
    connect(m_ui->spbQueryInterval, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setQueryInterval);
    connect(m_ui->cmbCameraMode, &QComboBox::currentTextChanged, &PreferenceProvider::instance(), &PreferenceProvider::setCameraMode);
    connect(m_ui->cmbCameraMode, &QComboBox::currentTextChanged, this, &Preferences::verifyApplication);
    connect(m_ui->cmbCameraMode, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [&]() {
        //QComboBox has changed, show stored data to QLineEdit
        m_ui->txtArgumentLine->setText(m_ui->cmbCameraMode->currentData().toString());
    });
    connect(m_ui->txtArgumentLine, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setArgumentLine);
    connect(m_ui->txtArgumentLine, &QLineEdit::textChanged, this, [&](const QString &i_value) {
        //save changed text in QComboBox model
        m_ui->cmbCameraMode->setItemData(m_ui->cmbCameraMode->currentIndex(), i_value);
    });
    connect(m_ui->spbTimout, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setTimeoutValue);
    connect(m_ui->chbGrayscale, &QAbstractButton::toggled, &PreferenceProvider::instance(), &PreferenceProvider::setGrayscale);

    //connect buttons
    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &Preferences::startFotoBox);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_ui->buttonBox, &QDialogButtonBox::clicked, this, [&](QAbstractButton *button) {
        //identify restore button and restore defaults
        if (button == m_ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
            restoreDefaultPreferences();
        }
    });
}

void Preferences::startFotoBox()
{
    //save settings to ini file
    savePreferences();
    m_countdown.stop();

    //Start FotoBox;
    auto dialog = new FotoBox;

    //close dialog and start fotobox
    reject();
    dialog->showFullScreen();
}

void Preferences::mouseMoveEvent(QMouseEvent *event)
{
    if (m_countdown.isActive()) {
        m_countdown.stop();
        setMouseTracking(false);
        m_ui->scrollArea->setMouseTracking(false);
        m_ui->scrollAreaWidgetContents->setMouseTracking(false);
        m_ui->tabWidget->setMouseTracking(false);
        m_ui->tabGenal->setMouseTracking(false);
        m_ui->tabExpert->setMouseTracking(false);
        setWindowTitle(tr("FotoBox preferences"));
    }

    //call base class method
    QWidget::mouseMoveEvent(event);
}

void Preferences::loadPreferences()
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
    m_ui->chbGrayscale->setChecked(m_settings.value(m_ui->chbGrayscale->objectName(), m_ui->chbGrayscale->isChecked()).toBool());
    m_settings.endGroup();
}

void Preferences::colorDialog()
{
    //"Color Picker" Dialog
    QColorDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        //show the color which the user has selected
        auto *button = qobject_cast<QPushButton *>(sender());
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

void Preferences::chooseDirectory()
{
    //File Dialog to choose photo folder
    QFileDialog dialog(this, tr("choose directory"), QDir::homePath());
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOptions(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    //only set it if user don't abort dialog
    if (dialog.exec() == QDialog::Accepted) {
        auto path = dialog.directory().absolutePath();
        m_ui->txtPhotoFolder->setText(QDir::toNativeSeparators(path));
    }
}

void Preferences::showColor(const QString &i_colorName)
{
    //create color
    QColor color(i_colorName);

    //set color (text + background)
    auto *lineEdit = qobject_cast<QLineEdit *>(sender());
    if (lineEdit != nullptr) {
        auto palette = lineEdit->palette();
        palette.setColor(QPalette::Text, color);
        palette.setColor(QPalette::Base, color);
        lineEdit->setPalette(palette);
        //force repaint (Restore Default issue)
        lineEdit->repaint();
    }
}

void Preferences::savePreferences()
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
    auto size = m_ui->cmbCameraMode->count();
    itemText.reserve(size);
    itemData.reserve(size);
    for (int i = 0; i < size; ++i) {
        itemText << m_ui->cmbCameraMode->itemText(i);
        itemData << m_ui->cmbCameraMode->itemData(i).toString();
    }
    m_settings.setValue(m_ui->cmbCameraMode->objectName() + QStringLiteral("Text"), itemText);
    m_settings.setValue(m_ui->cmbCameraMode->objectName() + QStringLiteral("Data"), itemData);
    m_settings.setValue(m_ui->cmbCameraMode->objectName(), PreferenceProvider::instance().cameraMode());

    m_settings.setValue(m_ui->spbTimout->objectName(), PreferenceProvider::instance().timeoutValue());
    m_settings.setValue(m_ui->chbGrayscale->objectName(), PreferenceProvider::instance().grayscale());
    m_settings.endGroup();
}

void Preferences::restoreDefaultPreferences()
{
    //FotoBox
    m_ui->txtPhotoFolder->setText(QDir::toNativeSeparators(QCoreApplication::applicationDirPath()));
    m_ui->txtPhotoName->setText(QStringLiteral("eventname.jpg"));
    m_ui->spbCountdown->setValue(3);
    m_ui->txtShowColorCD->setText(QStringLiteral("#ff0000"));
#if !defined(Q_OS_UNIX)
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
    m_ui->cmbCameraMode->addItem(QStringLiteral("gphoto2"),
                                 QLatin1String("--capture-image-and-download --keep --filename %1 --set-config "
                                               "/main/settings/capturetarget=1 --force-overwrite"));
    m_ui->cmbCameraMode->addItem(QStringLiteral("raspistill"), QLatin1String("--output %1 --width 1920 --height 1080 --quality 75 --nopreview --timeout 1"));
    m_ui->spbTimout->setValue(30);
    m_ui->chbGrayscale->setChecked(false);
}

void Preferences::verifyApplication(const QString &i_name)
{
    //gphoto2
    if (i_name == QLatin1String("gphoto2")) {
        auto message = tr("'%1' is missing%2")
                           .arg(i_name,
                                tr(": <a href='https://github.com/gonzalo/gphoto2-updater/'>Linux (gphoto2 "
                                   "updater)</a>/<a href='https://brew.sh/'>macOS (Homebrew)</a>"));
        if (applicationAvailable(i_name, message)) {
            //add info about gphoto2
            m_ui->lblCameraModeInfo->setText(gphotoInfo(i_name));
        }
        return;
    }

    //Raspberry Pi Camera Module
    if (i_name == QLatin1String("raspistill")) {
        auto message = tr("'%1' is missing%2")
                           .arg(i_name,
                                tr(": <a "
                                   "href='https://www.raspberrypi.org/documentation/usage/camera/"
                                   "'>Raspberry Pi Camera Module - enabling the camera</a>"));
        applicationAvailable(i_name, message);
        return;
    }

    //Other Applications
    auto message = tr("'%1' is missing%2").arg(i_name);
    applicationAvailable(i_name, message);
}

bool Preferences::applicationAvailable(const QString &i_name, const QString &i_message)
{
    //If the process cannot be started, -2 is returned. If the process crashes, -1 is returned.
    QProcess process;
    process.start(i_name, QIODevice::NotOpen);
    process.waitForFinished();
    if (process.exitStatus() < QProcess::NormalExit) {
        //set error message
        m_ui->lblCameraModeInfo->setStyleSheet(QStringLiteral("QLabel { color : red; }"));
        m_ui->lblCameraModeInfo->setText(i_message);
        m_ui->cmbCameraMode->setItemData(m_ui->cmbCameraMode->currentIndex(), QBrush(Qt::red), Qt::ForegroundRole);
        return false;
    }
    //application is available reset style and content
    m_ui->lblCameraModeInfo->setStyleSheet(QLatin1String(""));
    m_ui->lblCameraModeInfo->clear();
    m_ui->cmbCameraMode->setItemData(m_ui->cmbCameraMode->currentIndex(), QBrush(Qt::black), Qt::ForegroundRole);
    return true;
}

QString Preferences::gphotoInfo(const QString &i_name)
{
    QString result;

    //call 'gphoto2 --version' and get output
    QProcess process(this);
    process.start(i_name, {QStringLiteral("--version"), QStringLiteral("--summary")});
    process.waitForFinished();
    auto output = process.readAllStandardOutput();

    //^gphoto2\s{2,}(?<gphoto2>\d+\.\d+\.\d+).*\n^libgphoto2\s{2,}(?<libgphoto2>\d+\.\d+\.\d+)
    QString pattern(QStringLiteral("^gphoto2\\s{2,}(?<gphoto2>\\d+\\.\\d+\\.\\d+).*\\n^libgphoto2\\s{2,}(?<libgphoto2>\\d+\\.\\d+\\.\\d+)"));
    //use regex to get version string
    QRegularExpression regex(pattern, QRegularExpression::MultilineOption);
    auto match = regex.match(QString::fromLatin1(output));
    QString gphoto2(i_name);
    QString libgphoto2(QStringLiteral("libgphoto2"));
    if (match.hasMatch()) {
        gphoto2 += QStringLiteral(" v") + match.captured(gphoto2);
        libgphoto2 += QStringLiteral(" v") + match.captured(libgphoto2);
        result = gphoto2 + QStringLiteral(" / ") + libgphoto2;

        //^[Mm]odel:\s(.*$)
        pattern = QStringLiteral("^[Mm]odel:\\s(.*$)");
        //get camera model
        regex.setPattern(pattern);
        match = regex.match(QString::fromLatin1(output));
        result += QStringLiteral("\n");
        if (match.hasMatch()) {
            result += tr("camera model: %1").arg(match.captured(1));
        } else {
            result += tr("camera model: %1").arg(tr("NOT DETECTED"));
        }
    }

    return result;
}
