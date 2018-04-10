/* fotobox.cpp
 *
 * Copyright (c) 2017 Thomas Kais
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


FotoBox::FotoBox(QWidget *parent) : QDialog(parent),
  m_ui(new Ui::FotoBoxDialog),
  m_buzzer(new Buzzer),
  m_camera(this)
{
  //setup GUI
  m_ui->setupUi(this);

  //Fotobox process
  connect(this, &FotoBox::start, this, &FotoBox::startProcess);
  

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
  setStyleSheet(QStringLiteral("#FotoBoxDialog, #statusBar { background-color:%1; }").arg(PreferenceProvider::instance().backgroundColor()));

#ifdef __WIRING_PI_H__
  //running loop to check buzzer trigger
  connect(m_buzzer, &Buzzer::finished, this, &FotoBox::start);
  m_buzzer->start();
#endif
}


FotoBox::~FotoBox()
{
  //delete new
  delete m_ui;

  //delete Buzzer thread
  m_buzzer->deleteLater();
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

      //P KEY
      if (event->key() == Qt::Key_P) {
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
      m_ui->statusBar->showMessage(tr("Error: Taking a photo isn't working correctly! Please call the Fotobox owner."), 2000);
    }

  //restart Buzzer
  m_buzzer->start();
}

auto FotoBox::movePhoto() -> const QString
{
  const QString fileName = m_camera.currentPhoto();
  //old location of the photo
  const QString oldName = QCoreApplication::applicationDirPath() + QDir::separator() + fileName;
  //new location of the photo
  const QString newName = PreferenceProvider::instance().photoFolder() + QDir::separator() + fileName;

  //move photo
  QDir dir;
  if(dir.rename(oldName, newName)) {
      return newName;
    }
  else {
      m_ui->statusBar->showMessage(tr("Couldn't move the photo to: ") + newName, 3000);
      return oldName;
    }
}


auto FotoBox::loadPhoto(const QString& i_filePath) -> void
{
  if (m_photo.load(i_filePath)) {
      //resize picture to label size
      QSize size(m_ui->lblPhoto->width(), m_ui->lblPhoto->height());
      m_ui->lblPhoto->setPixmap(m_photo.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
  else {
      m_ui->statusBar->showMessage(tr("Couldn't load the photo."), 3000);
    }
}
