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
#include "camera.h"

#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QKeyEvent>


FotoBox::FotoBox(QWidget* parent) : QMainWindow(parent),
  m_ui(new Ui::MainWindow),
  m_buzzer(new Buzzer(nullptr))
{
  //Setup GUI
  m_ui->setupUi(this);

  //GUI Connect
  QObject::connect(m_ui->quitApp, &QPushButton::clicked, qApp, &QCoreApplication::quit);
  QObject::connect(m_ui->start, &QPushButton::clicked, this, &FotoBox::startShot);

  //Gphoto2 installed on the operating system?
  if(!checkGPhoto2())
  {
    std::exit(EXIT_FAILURE);
  }

  //Running loop to check buzzer trigger
  connect(m_buzzer, &Buzzer::finished, this, &FotoBox::startShot);
  m_buzzer->start();
}


auto FotoBox::keyPressEvent(QKeyEvent *event) -> void
{
  if(event->key() == Qt::Key_Return)
  {
    //Shot a Foto
    startShot();
  }
  if(event->key() == Qt::Key_Escape)
  {
    //Quit application
    qApp->quit();
  }
}


FotoBox::~FotoBox()
{
  //Delete new
  delete m_ui;

  //Delete Buzzer thread
  m_buzzer->quit();
  m_buzzer->deleteLater();
}


auto FotoBox::checkGPhoto2() -> const bool
{
  bool result = true;

  auto gphoto2 = new QProcess(this);
  gphoto2->start("gphoto2 --auto-detect");

  gphoto2->waitForFinished();
  auto output = gphoto2->readAll();

  //clean up
  gphoto2->deleteLater();

  if(output.isEmpty())
  {
    QMessageBox::critical(this, tr("gphoto2 not found"), tr("Please install 'gphoto2' on your Raspberry Pi\nhttps://github.com/gonzalo/gphoto2-updater"));
    result = false;
  }
  return result;
}


auto FotoBox::startShot() -> void
{
  Camera cam(this);

  //take a photo
  if(cam.takePicture())
  {
    showResults();

    //restart Buzzer
    m_buzzer->start();
  }
  else
  {
    QMessageBox::critical(this, tr("gphoto2 not found"), tr("Please install 'gphoto2' on your Raspberry Pi\nhttps://github.com/gonzalo/gphoto2-updater"));
  }
}


auto FotoBox::showResults() -> void
{
  //Resize picture
  QSize size(m_ui->picture->width(), m_ui->picture->height());

  QPixmap img(size);

  if (!img.load(qApp->applicationDirPath() + QDir::separator() + "capt0000.jpg"))
  {
    QMessageBox::critical(this, tr("IMG not available"), tr("Couldn't load the Image."));
  }

  //Resize
  m_ui->picture->setPixmap(img.scaled(size, Qt::KeepAspectRatio));
}
