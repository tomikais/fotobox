/* fotobox.cpp
 *
 * Copyright (c) 2016 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#include "fotobox.h"

#include "preferenceprovider.h"
#include "preferences.h"
#include "ui_fotobox.h"

#include <QDate>
#include <QDir>
#include <QKeyEvent>
#include <QPainter>

FotoBox::FotoBox(QWidget *parent)
    : QDialog(parent)
    , m_ui(new Ui::FotoBoxDialog)
    , m_countdown(this)
    , m_workerThread(this)
    , m_camera(this)
    , m_photoDir(PreferenceProvider::instance().photoFolder() + QDir::separator())
{
    //setup GUI
    m_ui->setupUi(this);

    //e.g. 'FotoBox v1.2.3 (Copyright 2016 Thomas Kais)'
    setWindowTitle(QStringLiteral("%1 v%2 (Copyright 2016 %3)").arg(QApplication::applicationName(), QApplication::applicationVersion(), QApplication::organizationName()));

    //set Background Color
    setStyleSheet(QStringLiteral("#FotoBoxDialog { background-color:%1; }").arg(PreferenceProvider::instance().backgroundColor()));

    //delete everything on close
    setAttribute(Qt::WA_DeleteOnClose);

    //show QStatusBar only when needed (safe space for the photos)
    connect(m_ui->statusBar, &QStatusBar::messageChanged, this, [&](const QString &i_message) { i_message.isNull() ? m_ui->statusBar->hide() : m_ui->statusBar->show(); });
    //hide status bar (ATTENTION: don't use at other places, see connect line above)
    m_ui->statusBar->hide();

    //with or without buttons?
    buttons();

    //Buzzer class (Raspberry Pi GPIO using wiringPi)
#if defined(BUZZER_AVAILABLE)
    if (PreferenceProvider::instance().queryInterval() > 0) {
        buzzer();
    }
#endif

    //countdown?
    countdown();
}

void FotoBox::buttons()
{
    if (PreferenceProvider::instance().showButtons()) {
        //connect buttons
        connect(m_ui->btnStart, &QPushButton::clicked, this, &FotoBox::start);
        connect(m_ui->btnPreferencesDialog, &QPushButton::clicked, this, &FotoBox::preferenceDialog);
        connect(m_ui->btnQuitApp, &QPushButton::clicked, QCoreApplication::instance(), &QCoreApplication::quit);
    } else {
        //hide mouse cursor
        QGuiApplication::setOverrideCursor(Qt::BlankCursor);
        //hide buttons
        m_ui->btnStart->hide();
        m_ui->btnPreferencesDialog->hide();
        m_ui->btnQuitApp->hide();
    }
}

void FotoBox::buzzer()
{
#if defined(BUZZER_AVAILABLE)
    if (m_workerThread.isRunning()) {
        return;
    }

    //move to a thread
    m_buzzer.moveToThread(&m_workerThread);

    //connect the start signale for buzzer
    connect(this, &FotoBox::startBuzzer, &m_buzzer, &Buzzer::queryPin);
    //start fotobox if buzzer was triggered
    connect(&m_buzzer, &Buzzer::triggered, this, &FotoBox::start);

    m_workerThread.start();

    //start query
    emit startBuzzer();
#endif
}


void FotoBox::countdown()
{
    //initialize countdown
    m_ui->lcdCountdown->hide();

    if (PreferenceProvider::instance().countdown() > 0) {
        //add countdown
        m_countdown.setStartTime(static_cast<unsigned int>(PreferenceProvider::instance().countdown()));
        connect(this, &FotoBox::start, &m_countdown, &Countdown::start);
        connect(&m_countdown, &Countdown::elapsed, this, &FotoBox::photo);

        //update UI
        connect(&m_countdown, &Countdown::update, this, [&](const unsigned int i_timeLeft) {
            //hide photo and show countdown
            m_ui->lblPhoto->hide();
            m_ui->lcdCountdown->show();
            m_ui->lcdCountdown->display(QString::number(i_timeLeft));
        });

        //apply font color
        auto palette = m_ui->lcdCountdown->palette();
        palette.setColor(QPalette::WindowText, PreferenceProvider::instance().countdownColor());
        m_ui->lcdCountdown->setPalette(palette);
    } else {
        //disable countdown
        connect(this, &FotoBox::start, this, &FotoBox::photo);
    }
}

FotoBox::~FotoBox()
{
#if defined(BUZZER_AVAILABLE)
    if (PreferenceProvider::instance().queryInterval() > 0) {
        //stop query pin
        m_buzzer.stop();
    }
#endif
    //terminate and delete Buzzer thread
    m_workerThread.quit();
    m_workerThread.wait();
}


void FotoBox::keyPressEvent(QKeyEvent *event)
{
    //prevent triggering method too often
    if (!event->isAutoRepeat()) {
        //start FotoBox
        if (std::any_of(m_triggerKey.cbegin(), m_triggerKey.cend(), [&](const Qt::Key i_key) { return event->key() == i_key; })) {
            Q_EMIT start();
            return;
        }

        //preferences dialog
        if (std::any_of(m_preferenceKey.cbegin(), m_preferenceKey.cend(), [&](const Qt::Key i_key) { return event->key() == i_key; })) {
            preferenceDialog();
            return;
        }

        //quit application
        if (std::any_of(m_quitKey.cbegin(), m_quitKey.cend(), [&](const Qt::Key i_key) { return event->key() == i_key; })) {
            if (event->modifiers() == Qt::ShiftModifier) {
                QCoreApplication::quit();
            }
            m_ui->statusBar->showMessage(tr("To quit the application, please hold down the Shift key while press Escape key."), STATUSBAR_MSG_TIMEOUT);
            return;
        }
    }
}

void FotoBox::mouseReleaseEvent(QMouseEvent *event)
{
    //touch support only when no buttons are shown
    if (!PreferenceProvider::instance().showButtons() && (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)) {
        Q_EMIT start();
        return;
    }

    QWidget::mouseReleaseEvent(event);
}

void FotoBox::preferenceDialog()
{
    //Preferences dialog
    auto dialog = new Preferences;

    //restore mouse cursor
    QApplication::restoreOverrideCursor();

    //close fotobox and show preferences
    reject();

#if defined(Q_OS_MACOS)
    //QTBUG-36714: Window can't be closed on Mac OS X after calling showFullScreen()
    closeFullscreenWindowOnMac();
#endif

    dialog->show();
}

void FotoBox::photo()
{
    //show label and hide other widgets
    m_ui->lcdCountdown->hide();
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    //remove current photo
    m_ui->lblPhoto->show();
    m_ui->lblPhoto->clear();
    m_ui->lblPhoto->repaint();

    //take a photo
    if (m_camera.shootPhoto()) {
        //load photo
        loadPhoto(m_photoDir + m_camera.currentPhoto());
    } else {
        m_ui->statusBar->showMessage(tr("Error: Taking a photo isn't working correctly!"), STATUSBAR_MSG_TIMEOUT);
    }

    //restart countdown and Buzzer
    if (PreferenceProvider::instance().countdown() > 0) {
        m_countdown.reset();
    }
#if defined(BUZZER_AVAILABLE)
    if (PreferenceProvider::instance().queryInterval() > 0) {
        emit startBuzzer();
    }
#endif
}

void FotoBox::loadPhoto(const QString &i_filePath)
{
    //try to load the photo shot by camera
    if (m_photo.load(i_filePath)) {
        //resize picture to label size
        QSize size(m_ui->lblPhoto->width(), m_ui->lblPhoto->height());

        //show photo in grayscale (monochrome photography)
        if (PreferenceProvider::instance().grayscale()) {
            const auto grey = m_photo.toImage().convertToFormat(QImage::Format_Grayscale8);
            m_photo.convertFromImage(grey);
        }

        //scale photo and show it immediately
        m_photo = m_photo.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        m_ui->lblPhoto->setPixmap(m_photo);
        m_ui->lblPhoto->repaint();
    } else {
        m_ui->statusBar->showMessage(tr("Couldn't load the photo."), STATUSBAR_MSG_TIMEOUT);
    }
}

void FotoBox::drawText(const QString &i_text)
{
    //painter begins painting the paint device immediately!
    QPainter painter(&m_photo);

    //set color and font
    painter.setPen(QPen(PreferenceProvider::instance().backgroundColor()));

    //calculate best font size
    QFont font = painter.font();
    const auto size = calculateFontSize(m_photo.rect().width(), painter.fontMetrics().boundingRect(i_text).width());
    font.setPointSizeF(font.pointSizeF() * size);
    painter.setFont(font);

    //draw text on image
    painter.drawText(m_photo.rect(), Qt::AlignCenter, i_text);

    m_ui->lblPhoto->setPixmap(m_photo);
}

double FotoBox::calculateFontSize(const double i_width, const double i_widthFont)
{
    auto factor = i_width / i_widthFont;
    if ((factor < 1) || (factor > COMPARE)) {
        return factor;
    }
    return 0.0;
}
