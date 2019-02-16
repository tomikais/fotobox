/* fotobox.cpp
 *
 * Copyright (c) 2018 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "fotobox.h"

#include "buzzer.h"
#include "preferenceprovider.h"
#include "preferences.h"
#include "ui_fotobox.h"

#include <QDir>
#include <QKeyEvent>
#include <QTimer>

#if defined (__arm__) && __has_include(<wiringPi.h>)
#include <wiringPi.h>
#endif


FotoBox::FotoBox(QWidget *parent) : QDialog(parent),
  m_ui(new Ui::FotoBoxDialog),
  m_buzzer(nullptr),
  m_timer(new QTimer(this)),
  m_countdown(PreferenceProvider::instance().countdown()),
  m_workerThread(this),
  m_camera(this),
  m_workingDir(QDir::currentPath() + QDir::separator()),
  m_photoDir(PreferenceProvider::instance().photoFolder() + QDir::separator())
{
  //setup GUI
  m_ui->setupUi(this);

  //delete everything on close
  setAttribute(Qt::WA_DeleteOnClose);

  //connect signal to corresponding slot
  connect(this, &FotoBox::startPhoto, this, &FotoBox::photo);
  connect(this, &FotoBox::startCountdown, this, &FotoBox::countdown);

  //show QStatusBar only when needed (safe space for the photos)
  connect(m_ui->statusBar, &QStatusBar::messageChanged, this, [&] (const QString &i_message) {
      i_message.isNull() ? m_ui->statusBar->hide() : m_ui->statusBar->show();
    });

  if (PreferenceProvider::instance().showButtons()) {
      //connect buttons
      connect(m_ui->btnStart, &QPushButton::clicked, this, &FotoBox::start);
      connect(m_ui->btnPreferencesDialog, &QPushButton::clicked, this, &FotoBox::preferenceDialog);
      connect(m_ui->btnQuitApp, &QPushButton::clicked, QCoreApplication::instance(), &QCoreApplication::quit);
    }
  else {
      //hide mouse cursor
      QGuiApplication::setOverrideCursor(Qt::BlankCursor);
      //hide buttons
      m_ui->btnStart->setVisible(false);
      m_ui->btnPreferencesDialog->setVisible(false);
      m_ui->btnQuitApp->setVisible(false);
    }

  //show countdown only when needed
  m_ui->lcdCountdown->setVisible(false);
  if (m_countdown == 0) {
      //disable countdown
      connect(this, &FotoBox::start, this, &FotoBox::startPhoto);
    }
  else {
      //initialize countdown
      m_timer->setInterval(PreferenceProvider::ONE_SECOND);
      connect(m_timer, &QTimer::timeout, this, &FotoBox::startCountdown);
      connect(this, &FotoBox::start, this, &FotoBox::startCountdown);

      //apply font color
      auto palette = m_ui->lcdCountdown->palette();
      palette.setColor(QPalette::WindowText, PreferenceProvider::instance().countdownColor());
      m_ui->lcdCountdown->setPalette(palette);
    }

  //set Background Color
  setStyleSheet(QStringLiteral("#FotoBoxDialog { background-color:%1; }").arg(PreferenceProvider::instance().backgroundColor()));
  //hide status bar
  m_ui->statusBar->hide();

  //Buzzer class (Raspberry Pi GPIO using wiringPi)
  buzzer();
}


FotoBox::~FotoBox()
{
  //terminate and delete Buzzer thread
  if (m_buzzer != nullptr) {
      //stop query pin
      m_buzzer->stop();
    }
  m_workerThread.quit();
  m_workerThread.wait();
}


void FotoBox::keyPressEvent(QKeyEvent *event)
{
  //prevent triggering method too often
  if (!event->isAutoRepeat()) {
      //ENTER key and ENTER on keypad
      if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
          //take a photo
          emit start();
        }

      //ESCAPE KEY
      if (event->key() == Qt::Key_Escape) {
          //Quit application
          QCoreApplication::quit();
        }

      //Preferences KEYS (P)references, (S)ettings or (E)instellungen
      if (event->key() == Qt::Key_P || event->key() == Qt::Key_S || event->key() == Qt::Key_E) {
          //show Preference Dialog
          preferenceDialog();
        }
    }
}


void FotoBox::mouseReleaseEvent(QMouseEvent *event)
{
  //touch support only when no buttons are shown
  if (!PreferenceProvider::instance().showButtons() && (event->button() == Qt::LeftButton || event->button() == Qt::RightButton)) {
      emit start();
    }

  QWidget::mouseReleaseEvent(event);
}


void FotoBox::buzzer()
{
  if (m_buzzer != nullptr) {
      //stop query pin
      m_buzzer->stop();
    }

  if (m_workerThread.isRunning()) {
      //stop QThread
      m_workerThread.quit();
      m_workerThread.wait();
    }

  //create Buzzer and move to a thread
  m_buzzer = new Buzzer;
  m_buzzer->moveToThread(&m_workerThread);

  //connect and start Buzzer
  connect(&m_workerThread, &QThread::finished, m_buzzer, &QObject::deleteLater);
  connect(this, &FotoBox::startBuzzer, m_buzzer, &Buzzer::queryPin);
  connect(m_buzzer, &Buzzer::triggered, this, &FotoBox::start);
  m_workerThread.start();

#if defined (__WIRING_PI_H__)
  //start query
  emit startBuzzer();
#endif
}


void FotoBox::preferenceDialog()
{
#if defined (Q_OS_MACOS)
  //QTBUG-36714: Window can't be fully closed on Mac OS X after calling showFullScreen()
  showNormal();
#endif

  //Preferences dialog
  auto dialog = new Preferences;

  //restore mouse cursor
  QApplication::restoreOverrideCursor();

  //close fotobox and show preferences
  reject();
  dialog->show();
}


void FotoBox::countdown()
{
  if (m_countdown >= 1) {
      //hide photo and show countdown
      m_ui->lblPhoto->setVisible(false);
      m_ui->lcdCountdown->setVisible(true);
      m_ui->lcdCountdown->display(m_countdown);

      --m_countdown;
      m_timer->start();
      return;
    }

  //reset counter, stop timer and hide countdown
  m_countdown = PreferenceProvider::instance().countdown();
  m_timer->stop();
  m_ui->lcdCountdown->setVisible(false);
  m_ui->lblPhoto->setVisible(true);

  //shot photo
  emit startPhoto();
}


void FotoBox::photo()
{
  //remove current picture / refresh label (photo)
  m_ui->lblPhoto->clear();
  m_ui->lblPhoto->repaint();

  //take a photo
  if (m_camera.shootPhoto()) {
      //load photo
      const QString filePath = movePhoto();
      loadPhoto(filePath);
    }
  else {
      m_ui->statusBar->showMessage(tr("Error: Taking a photo isn't working correctly! Please call the FotoBox owner."), STATUSBAR_MSG_TIMEOUT);
    }

  //restart Buzzer
  buzzer();
}

const QString FotoBox::movePhoto()
{
  //old location of the photo (working directory not application directory)
  const QString oldName = m_workingDir + m_camera.currentPhoto();;
  //new location of the photo
  const QString newName = m_photoDir + m_camera.currentPhoto();;

  //move photo only if working directory isn't same as preference folder
  if (QDir(oldName) != QDir(newName)) {
      //move photo
      if(QFile::rename(oldName, newName)) {
          //successfully moved the file/photo
          return newName;
        }
        //error handling
        m_ui->statusBar->showMessage(tr("Couldn't move the photo to: %1").arg(newName), STATUSBAR_MSG_TIMEOUT);
    }

  return oldName;
}


void FotoBox::loadPhoto(const QString& i_filePath)
{
  //try to load the photo shot by camera
  if (m_photo.load(i_filePath)) {
      //resize picture to label size
      QSize size(m_ui->lblPhoto->width(), m_ui->lblPhoto->height());
      m_ui->lblPhoto->setPixmap(m_photo.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
  else {
      m_ui->statusBar->showMessage(tr("Couldn't load the photo."), STATUSBAR_MSG_TIMEOUT);
    }
}
