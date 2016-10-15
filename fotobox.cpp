#include "fotobox.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTimer>


FotoBox::FotoBox(QWidget* parent) :
  QMainWindow(parent),
  status(true),
  ui(new Ui::MainWindow),
  countdown(new QTimer(this)),
  countdowntime(0),
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
    QMessageBox::critical(this, tr("gphoto2 not found"), tr("Please install 'gphoto2' on your Raspberry Pi.\n"
                                                            "https://github.com/gonzalo/gphoto2-updater"));
    status = false;
  }
}


FotoBox::~FotoBox()
{
  delete ui;
}


void FotoBox::startShot()
{
  //Timer Aktivieren
  countdown->start(SECOUND);

  //wiringPi
  //http://wiringpi.com/reference/setup/
  wiringPiSetupSys(); //da kein root

  //
  showResults();
}


void FotoBox::showResults()
{
  //Test Darstellunge Bilder
  QSize size(ui->firstImg->width(), ui->firstImg->height());

  QPixmap image(size);
  image.load("../firstImage.jpg");

  //Resize
  ui->firstImg->setPixmap(image.scaled(size, Qt::KeepAspectRatio));
}


void FotoBox::updateCountdown()
{
  ++countdowntime;
  if (countdowntime > countdowntimeout)
  {
    ui->lcdNumber->display(0);
    countdowntime = 0;
    countdown->stop();
  }
  else
  {
    ui->lcdNumber->display(countdowntime);
    countdown->start(SECOUND);
  }
}
