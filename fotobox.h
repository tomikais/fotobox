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
class Buzzer;


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
  * \brief FotoBox destructor
  */
  ~FotoBox();


private:
  /*!
   * \brief show the foto
   */
  auto showResults() -> void;

  /*!
   * \brief check if gphoto2 is installed on system
   */
  auto checkGPhoto2() -> const bool;

  /*!
   * \brief shot a picture
   */
  auto startShot() -> void;

  //User Interface
  Ui::MainWindow* m_ui;
  //Buzzer (Raspberry Pi GPIO)
  Buzzer* m_buzzer;
};
