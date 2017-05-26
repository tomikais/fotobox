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

#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QKeyEvent>


FotoBox::FotoBox(QWidget* parent) : QMainWindow(parent),
  m_ui(new Ui::MainWindow),
  m_camera(this),
  m_buzzer(new Buzzer(nullptr)),
  m_appPath(QApplication::applicationDirPath() + QDir::separator()),
  m_photo()
{
  //Setup GUI
  m_ui->setupUi(this);

#ifdef QT_DEBUG
  //connect buttons
  QObject::connect(m_ui->quitApp, &QPushButton::clicked, qApp, &QCoreApplication::quit);
  QObject::connect(m_ui->start, &QPushButton::clicked, this, &FotoBox::startShot);
#else //RELEASE
  //remove mouse cursor
  QApplication::setOverrideCursor(Qt::BlankCursor);
  //remove buttons
  m_ui->quitApp->deleteLater();
  m_ui->start->deleteLater();
#endif

  //gphoto2 installed on the operating system?
  if(!checkGPhoto2()) {
    //gphoto not found -> exit
    std::exit(EXIT_FAILURE);
  }

  //Running loop to check buzzer trigger
  connect(m_buzzer, &Buzzer::finished, this, &FotoBox::startShot);
  m_buzzer->start();
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


auto FotoBox::checkGPhoto2() -> const bool
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
    QMessageBox::critical(nullptr, tr("gphoto2 not found"),
                          tr("Please install 'gphoto2' on your Raspberry Pi\nhttps://github.com/gonzalo/gphoto2-updater"));
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
  m_ui->photo->clear();
  m_ui->photo->repaint();

  //take a photo
  if(m_camera.takePicture()) {
    //show picture on UI
    showResults();
  }
  else {
    QApplication::restoreOverrideCursor();
    QMessageBox::critical(nullptr, tr("Error"),
                          tr("Taking a photo isn't working correctly! Please call the Fotobox owner."));
#ifndef QT_DEBUG
    QApplication::setOverrideCursor(Qt::BlankCursor);
#endif
  }

  //restart Buzzer
  m_buzzer->start();
}


auto FotoBox::showResults() -> void
{
  //get size from label
  QSize size(m_ui->photo->width(), m_ui->photo->height());

  //load photo
  if (!m_photo.load(m_appPath + "capt0000.jpg")) {
    QApplication::restoreOverrideCursor();
    QMessageBox::critical(nullptr, tr("Load photo"), tr("Couldn't load the image."));
#ifndef QT_DEBUG
    QApplication::setOverrideCursor(Qt::BlankCursor);
#endif
  }

  //Resize picture
  m_ui->photo->setPixmap(m_photo.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
