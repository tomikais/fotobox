/* fotobox.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once
#include <QMainWindow>

class QProcess;
class QTimer;
class Buzzer;

#define SECOUND 1000
#define ENDLESS -1

namespace Ui
{
class MainWindow;
}


/*!
 * \brief The FotoBox class
 * Main class to control UI and controll the process
 */
class FotoBox : public QMainWindow
{
  Q_OBJECT

public:
  /*!
  * \brief FotoBox constructor
  * \param parent QObject
  */
  explicit FotoBox(QWidget* parent = nullptr);

  /*!
  * \brief destructor
  */
  ~FotoBox();

  //MEMBER
  bool status;

public slots:
  void startShot();
  void updateCountdown();

protected:


private:
  //METHODEN
  void showResults();

  // MEMBER
  Ui::MainWindow* ui; //User Interface
  QTimer* countdown;
  QProcess* gphoto2;
  Buzzer* m_workerThread;

  const int countdown_finished;
  int countdown_time;

};
