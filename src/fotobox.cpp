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

#if defined (__arm__)
#include <wiringPi.h>
#endif


int FotoBox::STATUSBAR_MSG_TIMEOUT = 4000;


FotoBox::FotoBox(QWidget *parent) : QDialog(parent),
  m_ui(new Ui::FotoBoxDialog),
  m_buzzer(nullptr),
  m_workerThread(this),
   m_camera(this),
  m_photo(),
  m_workingDir(QDir::currentPath() + QDir::separator()),
  m_photoDir(PreferenceProvider::instance().photoFolder() + QDir::separator())
{
  //setup GUI
  m_ui->setupUi(this);

  //delete everything on close
  setAttribute(Qt::WA_DeleteOnClose);

  //Fotobox process
  connect(this, &FotoBox::start, this, &FotoBox::startProcess);
  connect(m_ui->statusBar, &QStatusBar::messageChanged, this, [&] (const QString &i_message) {
      //show QStatusBar only when needed (safe space for the photos)
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


auto FotoBox::keyPressEvent(QKeyEvent *event) -> void
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


auto FotoBox::mouseReleaseEvent(QMouseEvent *event) -> void
{
  //touch support
  if (!PreferenceProvider::instance().showButtons() && event->button() == Qt::LeftButton) {
      emit start();
    }

  QWidget::mouseReleaseEvent(event);
}


auto FotoBox::buzzer() -> void
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
  connect(m_buzzer, &Buzzer::triggered, this, &FotoBox::startProcess);
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


auto FotoBox::startProcess() -> void
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

auto FotoBox::movePhoto() -> const QString
{
  //get name of the photo
  QString result = m_camera.currentPhoto();
  //old location of the photo (working directory not application directory)
  const QString oldName = m_workingDir + result;
  //new location of the photo
  const QString newName = m_photoDir + result;

  //move photo
  QDir dir;
  if(dir.rename(oldName, newName)) {
      //successfully moved the file/photo
      result = newName;
    }
  else {
      //error handling
      m_ui->statusBar->showMessage(tr("Couldn't move the photo to: ") + newName, STATUSBAR_MSG_TIMEOUT);
      result = oldName;
    }

  return result;
}


auto FotoBox::loadPhoto(const QString& i_filePath) -> void
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
