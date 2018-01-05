/* fotobox.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "fotobox.h"

#include "ui_mainwindow.h"
#include "buzzer.h"

#include <QProcess>
#include <QKeyEvent>
#include <QTimer>
#include <QMessageBox>


FotoBox::FotoBox(QWidget *parent) : QMainWindow(parent),
  m_ui(new Ui::MainWindow),
  m_camera(this),
  m_buzzer(new Buzzer),
  m_photo(),
  m_preferences(this)
{
  //Setup GUI
  m_ui->setupUi(this);

#ifdef QT_DEBUG
  //connect buttons
  connect(m_ui->btnQuitApp, &QPushButton::clicked, qApp, &QCoreApplication::quit);
  connect(m_ui->btnStart, &QPushButton::clicked, this, &FotoBox::startShot);
#else //RELEASE
  //remove mouse cursor
  QApplication::setOverrideCursor(Qt::BlankCursor);
  //remove buttons
  m_ui->quitApp->deleteLater();
  m_ui->start->deleteLater();
#endif

  //gphoto2 installed on the operating system?
#if defined __APPLE__ || defined __linux__
  if(!checkGPhoto2()) {
      //gphoto not found -> exit
      std::exit(EXIT_FAILURE);
    }
#endif

  //Running loop to check buzzer trigger
#ifdef __arm__
  connect(m_buzzer, &Buzzer::finished, this, &FotoBox::startShot);
#endif
  m_buzzer->start();

  m_preferences.show();
}


FotoBox::~FotoBox()
{
  //Delete new
  delete m_ui;

  //Delete Buzzer thread
  m_buzzer->deleteLater();
}


auto FotoBox::keyPressEvent(QKeyEvent *event) -> void
{
  //ESCAPE KEY
  if(event->key() == Qt::Key_Return) {
      //Shot a Foto
      startShot();
    }

  //ENTER KEY
  if(event->key() == Qt::Key_Escape) {
      //Quit application
      QApplication::quit();
    }
}


auto FotoBox::checkGPhoto2() -> bool
{
  bool result = true;

  //Process who starts gphoto2
  auto process = new QProcess(this);

  //auto-detect: List auto-detected cameras
  process->start("gphoto2 --auto-detect");

  //sync call
  process->waitForFinished();
  auto output = process->readAll();

  //check result
  if(output.isEmpty()) {
      QApplication::restoreOverrideCursor();
      QMessageBox msgBox;
      msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
      msgBox.setWindowTitle("gphoto2 missing");
      msgBox.setText(tr("Please install 'gphoto2' on <br><br>"
                        "Raspberry Pi: <a href='https://github.com/gonzalo/gphoto2-updater'>gphoto2-updater</a><br>"
                        "macOS: <a href='https://brew.sh>Homebrew/'>Homebrew</a>"));
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.exec();
#ifndef QT_DEBUG
      QApplication::setOverrideCursor(Qt::BlankCursor);
#endif

      result = false;
    }

  //clean up
  process->deleteLater();

  return result;
}


auto FotoBox::startShot() -> void
{
  //remove current picture / refresh label (photo)
  m_ui->lblPhoto->clear();
  m_ui->lblPhoto->repaint();

  //take a photo
  if(m_camera.takePicture()) {
      //show picture on UI
      showResults();
    }
  else {
      m_ui->statusBar->showMessage(tr("Error: Taking a photo isn't working correctly! Please call the Fotobox owner."), 4000);
    }

  //restart Buzzer
  m_buzzer->start();
}


auto FotoBox::showResults() -> void
{
  //get size from label
  QSize size(m_ui->lblPhoto->width(), m_ui->lblPhoto->height());

  //load photo
  if (!m_photo.load(m_preferences.pictureDirectory() + "preview.jpg")) {
      m_ui->statusBar->showMessage(tr("Couldn't load the image."), 3000);
    }
  else {
      //Resize picture
      m_ui->lblPhoto->setPixmap(m_photo.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
