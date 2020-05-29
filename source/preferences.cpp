/* preferences.cpp
 *
 * Copyright (c) 2018 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#include "fotobox.h"

#include "preferenceprovider.h"
#include "preferences.h"
#include "ui_commandlineoptions.h"
#include "ui_preferences.h"

#include <QColorDialog>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinterInfo>
#include <QProcess>
#include <QScreen>

Preferences::Preferences(QWidget *parent)
    : QDialog(parent, Qt::Window)
    , m_ui(new Ui::PreferencesDialog)
    , m_settings(QSettings::IniFormat, QSettings::UserScope, QCoreApplication::applicationName(), QCoreApplication::applicationName(), this)
    , m_countdown(this, COUNTDOWN_START_VALUE)
{
    //setup UI
    m_ui->setupUi(this);

    //set icons for QToolButtons
    setButtonIcons();

    //set window position
    windowPosition();

    //delete everything on close
    setAttribute(Qt::WA_DeleteOnClose);

    //DON'T CHANGE ORDER!
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
        //: %1 FotoBox %2 application version %3 countdown (number)
        setWindowTitle(tr("launching %1 v%2 in %3 seconds").arg(QApplication::applicationName(), QApplication::applicationVersion()).arg(i_timeLeft));
    });
}

Preferences::~Preferences()
{
    delete m_ui;
}

void Preferences::windowPosition()
{
    const auto availableGeometry = QGuiApplication::primaryScreen()->availableGeometry();
    if (frameGeometry().height() > availableGeometry.height()) {
        //start maximized to be able to reach the dialog buttons
        showMaximized();
    } else {
        //enough space, center dialog
        setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), availableGeometry));
    }
}

void Preferences::connectUi()
{
    //stop countdown if the application isn't visible and selected to be in front.
    connect(qobject_cast<QApplication*>(QCoreApplication::instance()), &QGuiApplication::applicationStateChanged, this, [&](const Qt::ApplicationState state) {
        if (state != Qt::ApplicationActive) {
            stopCountdownMode();
        }
    });

    //connect UI to preferences
    connect(m_ui->txtPhotoFolder, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setPhotoFolder);
    connect(m_ui->txtPhotoFolder, &QLineEdit::textChanged, this, &Preferences::verifyPath);
    connect(m_ui->btnChooseDirectory, &QPushButton::clicked, this, &Preferences::chooseDirectory);
    connect(m_ui->btnClearDirectoryContent, &QPushButton::clicked, this, &Preferences::clearDirectoryContent);
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
    connect(m_ui->cmbCameraMode, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [&]() {
        //QComboBox has changed, show stored data to QLineEdit
        m_ui->txtArgumentLine->setText(m_ui->cmbCameraMode->currentData().toString());
    });
    connect(m_ui->cmbCameraMode, &QComboBox::currentTextChanged, this, &Preferences::verifyApplication);
    connect(m_ui->btnCameraModeReload, &QAbstractButton::clicked, this, [&]() {
        verifyApplication(m_ui->cmbCameraMode->currentText());
    });
    connect(m_ui->txtArgumentLine, &QLineEdit::textChanged, &PreferenceProvider::instance(), &PreferenceProvider::setArgumentLine);
    connect(m_ui->txtArgumentLine, &QLineEdit::textChanged, this, [&](const QString &i_value) {
        //save changed text in QComboBox model
        m_ui->cmbCameraMode->setItemData(m_ui->cmbCameraMode->currentIndex(), i_value);
    });
    connect(m_ui->btnArgumentLineHelp, &QAbstractButton::clicked, this, &Preferences::commandLineOptionsDialog);
    connect(m_ui->spbTimout, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), &PreferenceProvider::instance(), &PreferenceProvider::setTimeoutValue);
    connect(m_ui->chbGrayscale, &QAbstractButton::toggled, &PreferenceProvider::instance(), &PreferenceProvider::setGrayscale);
    connect(m_ui->chbPrint, &QAbstractButton::toggled, &PreferenceProvider::instance(), &PreferenceProvider::setPrint);
    connect(m_ui->cmbPrinterName, &QComboBox::currentTextChanged, &PreferenceProvider::instance(), &PreferenceProvider::setPrinterName);

    //connect buttons
    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &Preferences::startFotoBox);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_ui->buttonBox, &QDialogButtonBox::clicked, this, [&](QAbstractButton *button) {
        //identify restore button, restore defaults and save it
        if (button == m_ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
            restoreDefaultPreferences();
            m_settings.sync();
        }
    });
    connect(m_ui->buttonBox, &QDialogButtonBox::helpRequested, this, [&]() {
        //request OS to open the URL
        QDesktopServices::openUrl( { QStringLiteral("https://gitlab.com/tomikais/fotobox/blob/master/README.md") } );
    });
}

void Preferences::setButtonIcons()
{
    //picture of all Qt standard icons: https://joekuan.files.wordpress.com/2015/09/screen3.png
    m_ui->btnChooseDirectory->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
    m_ui->btnClearDirectoryContent->setIcon(style()->standardIcon(QStyle::SP_DialogDiscardButton));
    m_ui->btnCameraModeReload->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    m_ui->btnArgumentLineHelp->setIcon(style()->standardIcon(QStyle::SP_TitleBarContextHelpButton));
}

void Preferences::startFotoBox()
{
    //save settings to ini file
    m_countdown.stop();
    savePreferences();

    //Start FotoBox;
    auto dialog = new FotoBox;

    //close dialog and start fotobox
    reject();
    dialog->showFullScreen();
}

void Preferences::mouseMoveEvent(QMouseEvent *event)
{
    stopCountdownMode();

    //call base class method
    QWidget::mouseMoveEvent(event);
}

void Preferences::stopCountdownMode()
{
    if (m_countdown.isActive()) {
        //stop countdown
        m_countdown.stop();

        //disable mouse tracking (not needed anymore)
        setMouseTracking(false);
        m_ui->scrollArea->setMouseTracking(false);
        m_ui->scrollAreaWidgetContents->setMouseTracking(false);
        m_ui->tabWidget->setMouseTracking(false);
        m_ui->tabGeneral->setMouseTracking(false);
        m_ui->tabExpert->setMouseTracking(false);
        m_ui->buttonBox->setMouseTracking(false);

        //set normal window title
        setWindowTitle(QStringLiteral("%1 v%2 (Copyright 2016 %3) - ").arg(QApplication::applicationName(), QApplication::applicationVersion(), QApplication::organizationName()) + tr("preferences"));
    }
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
    const auto data = m_settings.value(m_ui->cmbCameraMode->objectName() + QStringLiteral("Data")).toStringList();
    const auto text = m_settings.value(m_ui->cmbCameraMode->objectName() + QStringLiteral("Text")).toStringList();
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

    m_settings.beginGroup(QStringLiteral("PrintSetup"));
    m_ui->chbPrint->setChecked(m_settings.value(m_ui->chbPrint->objectName(), m_ui->chbPrint->isChecked()).toBool());
    const auto &printerName = m_settings.value(m_ui->cmbPrinterName->objectName(), m_ui->cmbPrinterName->currentText()).toString();
    const auto printer = m_ui->cmbPrinterName->findText(printerName);
    if (printer >= 0) {
        //printer still available so set it
        m_ui->cmbPrinterName->setCurrentIndex(printer);
    }
    m_settings.endGroup();
}

void Preferences::colorDialog()
{
    //"Color Picker" Dialog
    QColorDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        //show the color which the user has selected
        const auto *button = qobject_cast<QToolButton *>(sender());
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

void Preferences::commandLineOptionsDialog()
{
    //'Qt::Tool' act like a window 'Qt::ToolTip' always on top
    auto dialog = new QDialog(this, Qt::SplashScreen);
    Ui::CommandLineOptionsDialog ui;

    //setup UI
    ui.setupUi(dialog);

    //blocking until the user closes it
    dialog->exec();
}

void Preferences::chooseDirectory()
{
    //File Dialog to choose photo folder
    QFileDialog dialog(this, tr("choose directory"), m_ui->txtPhotoFolder->text());
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOptions(QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    //only set it if user don't abort dialog
    if (dialog.exec() == QDialog::Accepted) {
        const auto path = dialog.directory().absolutePath();
        if (!verifyPath(path)) {
            //recursion
            chooseDirectory();
            return;
        }
        //set path will again call verifyPath() see Signal&Slot connection
        m_ui->txtPhotoFolder->setText(QDir::toNativeSeparators(path));
    }
}

void Preferences::clearDirectoryContent()
{
    //get photo folder from preferences
    const QString photoFolder(PreferenceProvider::instance().PreferenceProvider::photoFolder());

    //ask user
    const auto &text = tr("Clear all JPEGs in the photo folder (leave subfolders untouched)?") + QStringLiteral("\n\"") + photoFolder + '\"';
    const auto &result = QMessageBox::warning(this, tr("WARNING: clear directory content"), text, QMessageBox::Yes | QMessageBox::No);

    if (result == QMessageBox::Yes) {
        //photo folder
        QDir dir(photoFolder);

        //JPEG file extension https://en.wikipedia.org/wiki/JPEG
        dir.setNameFilters({ QStringLiteral("*.jpg"), QStringLiteral("*.jpeg"), QStringLiteral("*.jpe"), QStringLiteral("*.jif"), QStringLiteral("*.jfif"), QStringLiteral("*.jfi") });

        const auto &entryList = dir.entryList(QDir::NoDotAndDotDot | QDir::Files);
        for(const auto &dirItem : entryList)
        {
            //remove all files
            dir.remove(dirItem);
        }
    }
}

bool Preferences::verifyPath(const QString &i_path)
{
    QFileInfo path(i_path);

    //check if path exists
    if (!path.exists()) {
        //create dir
        QDir dir(i_path);
        if (!dir.mkpath(QStringLiteral("."))) {
            QMessageBox::warning(this, tr("photo folder") , tr("The directory doesn't exist and also couldn't be created."));
            return false;
        }
    }

    //check if a directory
    if (!path.isDir()) {
        QMessageBox::warning(this, tr("photo folder") , tr("Please select a directory and not a file."));
        return false;
    }

    //check if readale and writable
    if (!path.isReadable() || !path.isWritable()) {
        QMessageBox::warning(this, tr("photo folder") , tr("Write and read rights are required. Please check the permission of the directory."));
        return false;
    }

    return true;
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
    //save QComboBox model
    QStringList itemText;
    QStringList itemData;
    const auto size = m_ui->cmbCameraMode->count();
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

    m_settings.beginGroup(QStringLiteral("PrintSetup"));
    m_settings.setValue(m_ui->chbPrint->objectName(), PreferenceProvider::instance().print());
    m_settings.setValue(m_ui->cmbPrinterName->objectName(), PreferenceProvider::instance().printerName());
    m_settings.endGroup();
}

void Preferences::restoreDefaultPreferences()
{
    //FotoBox
    m_ui->txtPhotoFolder->setText(QDir::toNativeSeparators(QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)) + QDir::separator() + QApplication::applicationName());
    m_ui->txtPhotoName->setText(QStringLiteral("eventname.jpg"));
    m_ui->spbCountdown->setValue(DEFAULT_COUNTDOWN_START_VALUE);
    m_ui->txtShowColorCD->setText(QStringLiteral("#ff0000"));
    m_ui->chbButtons->setChecked(true);
    m_ui->txtShowColorBG->setText(QStringLiteral("#000000"));

    //Buzzer
    m_ui->spbInputPin->setValue(DEFAULT_INPUT_PIN);
    m_ui->spbOutputPin->setValue(DEFAULT_OUTPUT_PIN);
    m_ui->spbQueryInterval->setValue(DEFAULT_QUERY_INTERVAL);

    //Camera
    m_ui->cmbCameraMode->clear();
    m_ui->cmbCameraMode->addItem(QStringLiteral("gphoto2"), QLatin1String("--capture-image-and-download --keep --force-overwrite --filename %1"));
    m_ui->cmbCameraMode->addItem(QStringLiteral("raspistill"), QLatin1String("--output %1 --width 1920 --height 1080 --quality 75 --nopreview --timeout 1"));
    m_ui->spbTimout->setValue(DEFAULT_TIMEOUT);
    m_ui->chbGrayscale->setChecked(false);

    //Print Setup
    //get all printers and preselect default printer
    m_ui->cmbPrinterName->clear();
    m_ui->cmbPrinterName->addItems(QPrinterInfo::availablePrinterNames());
    auto defaultPrinter = m_ui->cmbPrinterName->findText(QPrinterInfo::defaultPrinterName());
    m_ui->cmbPrinterName->setCurrentIndex(defaultPrinter);
    m_ui->chbPrint->setChecked(true);
}

void Preferences::verifyApplication(const QString &i_name)
{
    //stop here when clearing the camera mode QComboBox
    if (i_name.isEmpty()) {
        return;
    }

    //gphoto2
    if (i_name == QLatin1String("gphoto2")) {
        const auto message = tr("'%1' is missing%2")
                                 .arg(i_name, tr(": <a href='https://github.com/gonzalo/gphoto2-updater/'>Linux (gphoto2 updater)</a>"
                                                 "/<a href='https://brew.sh/'>macOS (Homebrew)</a>"));
        if (applicationAvailable(i_name, message)) {
            //add info about gphoto2
            m_ui->lblCameraModeInfo->setText(gphotoInfo(i_name));
        }
        return;
    }

    //Raspberry Pi Camera Module
    if (i_name == QLatin1String("raspistill")) {
        const auto message = tr("'%1' is missing%2")
                                 .arg(i_name, tr(": <a href='https://www.raspberrypi.org/documentation/usage/camera/'>"
                                                 "Raspberry Pi Camera Module - enabling the camera</a>"));
        applicationAvailable(i_name, message);
        return;
    }

    //Other Applications
    const auto message = tr("'%1' is missing%2").arg(i_name, QLatin1String(""));
    applicationAvailable(i_name, message);
}

bool Preferences::applicationAvailable(const QString &i_name, const QString &i_message)
{
    //If the process cannot be started, -2 is returned. If the process crashes, -1 is returned.
    QProcess process;
    process.start(i_name, {}, QIODevice::NotOpen);
    process.waitForFinished();
    const auto EXIT_CODE_OUT_OF_RANGE = 255;
    if (process.exitCode() == EXIT_CODE_OUT_OF_RANGE) {
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

    //call 'gphoto2 --version --summary' and get output in english
    QProcess process(this);
    auto env = QProcessEnvironment::systemEnvironment();
    env.insert(QStringLiteral("LC_ALL"), QStringLiteral("C"));
    process.setProcessEnvironment(env);
    process.start(i_name, {QStringLiteral("--version"), QStringLiteral("--summary")});
    process.waitForFinished();
    const auto output = QString::fromLatin1(process.readAllStandardOutput());

    //^gphoto2\s{2,}(?<gphoto2>\d+\.\d+\.\d+).*\n^libgphoto2\s{2,}(?<libgphoto2>\d+\.\d+\.\d+)
    QString pattern(QStringLiteral("^gphoto2\\s{2,}(?<gphoto2>\\d+\\.\\d+\\.\\d+).*\\n^libgphoto2\\s{2,}(?<libgphoto2>\\d+\\.\\d+\\.\\d+)"));
    //use regex to get version string
    QRegularExpression regex(pattern, QRegularExpression::MultilineOption);
    auto match = regex.match(output);
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
        match = regex.match(output);
        result += QStringLiteral("\n");
        if (match.hasMatch()) {
            const auto matches = match.lastCapturedIndex();
            result += tr("camera model: %1").arg(match.captured(1));
            //more then one camera detected ('0' full match, '1' first match, '2' second match,...)
            if (matches > 2) {
                for (int i = 2; i <= matches; ++i) {
                    result += QStringLiteral(" / ") + match.captured(i);
                }
            }
        } else {
            result += tr("camera model: %1").arg(tr("NOT DETECTED"));
        }
    }

    return result;
}
