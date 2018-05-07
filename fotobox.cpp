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
  m_camera(this)
{
  //setup GUI
  m_ui->setupUi(this);

  //Fotobox process
  connect(this, &FotoBox::start, this, &FotoBox::startProcess);
  connect(m_ui->statusBar, &QStatusBar::messageChanged, this, [this] (const QString &i_message) {
      //show QStatusBar only when needed (safe space for the photos)
      i_message.isNull() ? m_ui->statusBar->hide() : m_ui->statusBar->show();
    });

  if (PreferenceProvider::instance().showButtons()) {
      //connect buttons
      connect(m_ui->btnStart, &QPushButton::clicked, this, &FotoBox::start);
      connect(m_ui->btnPreferencesDialog, &QPushButton::clicked, this, &QDialog::reject);
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

  buzzer();
}


FotoBox::~FotoBox()
{
  //delete new
  delete m_ui;

  //terminate and delete Buzzer thread
  if (m_buzzer != nullptr && m_buzzer->isRunning()) {
      m_buzzer->requestInterruption();
      m_buzzer->deleteLater();
    }
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
          reject();
        }
    }
}


auto FotoBox::mouseReleaseEvent(QMouseEvent *event) -> void
{
  if (!PreferenceProvider::instance().showButtons() && event->button() == Qt::LeftButton) {
      emit start();
    }

  QWidget::mouseReleaseEvent(event);
}


auto FotoBox::buzzer() -> void
{
  //Buzzer class (Raspberry Pi using wiringPi)
#if defined (__WIRING_PI_H__)
  m_buzzer = new Buzzer(this);
  connect(m_buzzer, &Buzzer::finished, this, &FotoBox::start);
  connect(m_buzzer, &Buzzer::finished, m_buzzer, &QObject::deleteLater);
  connect(this, &FotoBox::rejected, m_buzzer, &QObject::deleteLater);
  m_buzzer->start();
#endif
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
  QString result = m_camera.currentPhoto();
  //old location of the photo (working directory not application directory)
  const QString oldName = QDir::currentPath() + QDir::separator() + result;
  //new location of the photo
  const QString newName = PreferenceProvider::instance().photoFolder() + QDir::separator() + result;

  //move photo
  QDir dir;
  if(dir.rename(oldName, newName)) {
      result = newName;
    }
  else {
      m_ui->statusBar->showMessage(tr("Couldn't move the photo to: ") + newName, STATUSBAR_MSG_TIMEOUT);
      result = oldName;
    }

  return result;
}


auto FotoBox::loadPhoto(const QString& i_filePath) -> void
{
  if (m_photo.load(i_filePath)) {
      //resize picture to label size
      QSize size(m_ui->lblPhoto->width(), m_ui->lblPhoto->height());
      m_ui->lblPhoto->setPixmap(m_photo.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
  else {
      m_ui->statusBar->showMessage(tr("Couldn't load the photo."), STATUSBAR_MSG_TIMEOUT);
    }
}
