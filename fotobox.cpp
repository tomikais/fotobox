/* fotobox.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "fotobox.h"

#include "ui_fotobox.h"
#include "buzzer.h"
#include "preferences.h"

#include <QProcess>
#include <QKeyEvent>
#include <QMessageBox>


FotoBox::FotoBox(QWidget *parent) : QMainWindow(parent),
  m_ui(new Ui::Fotobox),
  m_buzzer(new Buzzer),
  m_camera(this),
  m_photo()
{
  //setup GUI
  m_ui->setupUi(this);

  if (Preferences::getInstance().showButtons()) {
      //connect buttons
      connect(m_ui->btnQuitApp, &QPushButton::clicked, qApp, &QCoreApplication::quit);
      connect(m_ui->btnStart, &QPushButton::clicked, this, &FotoBox::startShot);
    }
  else {
      //hide mouse cursor
      QApplication::setOverrideCursor(Qt::BlankCursor);
      //hide buttons
      m_ui->btnQuitApp->setVisible(false);
      m_ui->btnStart->setVisible(false);
    }

  //set Background Color
  setStyleSheet(QString("#MainWindow, #statusBar { background-color:%1; }").arg(Preferences::getInstance().backgroundColor()));

#ifdef __arm__
  //running loop to check buzzer trigger
  connect(m_buzzer, &Buzzer::finished, this, &FotoBox::startShot);
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
  //ENTER key and ENTER on keypad
  if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
      //take a photo
      startShot();
    }

  //ESCAPE KEY
  if (event->key() == Qt::Key_Escape) {
      //Quit application
      qApp->quit();
    }
}


auto FotoBox::checkGPhoto2() -> bool
{
  //auto-detect: get detected cameras
  auto result = QProcess::execute("gphoto2", { "--auto-detect", "--version" });

  //check result
  if (result != EXIT_SUCCESS) {
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

      return false;
    }

  return true;
}


auto FotoBox::startShot() -> void
{
  //remove current picture / refresh label (photo)
  m_ui->lblPhoto->clear();
  m_ui->lblPhoto->repaint();

  //take a photo
  if (m_camera.shootPhoto()) {
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
  if (!m_photo.load(Preferences::getInstance().pictureDirectory() + "preview.jpg")) {
      m_ui->statusBar->showMessage(tr("Couldn't load the image."), 3000);
    }
  else {
      //resize picture to label size
      m_ui->lblPhoto->setPixmap(m_photo.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}
