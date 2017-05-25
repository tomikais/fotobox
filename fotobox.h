/* fotobox.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once
#include <QMainWindow>

#include "camera.h"

class QProcess;
class Buzzer;
class QKeyEvent;


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

protected:
  /*!
   * \brief keyPressEvent
   * \details
   * Enter -> take a photo
   * Escape -> quit application
   * \param event
   * \return void
   */
  auto keyPressEvent(QKeyEvent *event) -> void Q_DECL_OVERRIDE;

private:
  /*!
   * \brief show the foto
   * \return void
   */
  auto showResults() -> void;

  /*!
   * \brief check if gphoto2 is installed on system
   * return if gphoto2 is available
   */
  auto checkGPhoto2() -> const bool;

  /*!
   * \brief shot a picture
   * \return void
   */
  auto startShot() -> void;

  Camera m_camera;

  //User Interface
  Ui::MainWindow* m_ui;

  //Buzzer (Raspberry Pi GPIO)
  Buzzer* m_buzzer;
};
