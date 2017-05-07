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
#include <QTimer>
#include <QProcess>


FotoBox::FotoBox(QWidget* parent) :
  QMainWindow(parent),
  status(true),
  ui(new Ui::MainWindow),
  countdown(new QTimer(this)),
  countdown_finished(0),
  countdown_time(3), //TODO settings
  gphoto2(new QProcess(this))
{
  //Setup GUI
  ui->setupUi(this);

  //GUI Connect
  QObject::connect(ui->quitApp, SIGNAL(clicked()), qApp, SLOT(quit()));
  QObject::connect(ui->start, SIGNAL(clicked()), this, SLOT(startShot()));
  QObject::connect(countdown, SIGNAL(timeout()), this, SLOT(updateCountdown()));

  //gphoto2 auf System vorhanden?
  gphoto2->start("gphoto2 --auto-detect");
  gphoto2->waitForFinished(ENDLESS);
  QByteArray output = gphoto2->readAll();
  if(output.isEmpty())
  {
    QMessageBox::critical(this, tr("gphoto2 not found"), tr("Please install 'gphoto2' on your Raspberry Pi\nhttps://github.com/gonzalo/gphoto2-updater"));
    status = false;
  }
  gphoto2->deleteLater();

  //Running loop
  m_workerThread = new Buzzer(this);
  connect(m_workerThread, &Buzzer::finished, this, &FotoBox::startShot);
  m_workerThread->start();
}


FotoBox::~FotoBox()
{
  delete ui;

  //Delete Buzzer thread
  m_workerThread->exit();
  m_workerThread->deleteLater();
}


void FotoBox::startShot()
{
  //Timer Aktivieren
  countdown->start(SECOUND);

  //display picture
  connect(countdown, &QTimer::timeout, this, &FotoBox::showResults);
  //showResults();

  //restart Buzzer
  m_workerThread->start();
}


void FotoBox::showResults()
{
  Camera cam(this);
  cam.takePicture();

  //Test Darstellunge Bilder
  QSize size(ui->firstImg->width(), ui->firstImg->height());

  QPixmap image(size);

  if (!image.load(qApp->applicationDirPath() + QDir::separator() + "capt0000.jpg"))
  {
    QMessageBox::critical(this, tr("IMG"), tr("Couldn't load the Image."));

  }
  //Resize
  ui->firstImg->setPixmap(image.scaled(size, Qt::KeepAspectRatio));
}


void FotoBox::updateCountdown()
{
  --countdown_time;
  if (countdown_time > countdown_finished)
  {
    //Show countdown on screen
    ui->lcdNumber->display(countdown_time);
    countdown->setInterval(3000);
    countdown->start(SECOUND);
  }
  else
  {
    //RESET
    ui->lcdNumber->display(0);
    countdown_time = 3; //TODO settings
    countdown->stop();
  }
}

