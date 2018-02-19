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
class QKeyEvent;
class QTranslator;
class Buzzer;

namespace Ui
{
  class Fotobox;
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
  explicit FotoBox(QWidget *parent = nullptr);

  /*!
  * \brief FotoBox destructor
  */
  ~FotoBox();

  /*!
   * \brief check if gphoto2 is installed on system
   * return if gphoto2 is available
   */
  auto checkGPhoto2() -> bool;


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
   * \brief shot a picture
   * \return void
   */
  auto startShot() -> void;

  /*!
   * \brief show the foto
   * \return void
   */
  auto showResults() -> void;


  //User Interface
  Ui::Fotobox *m_ui;

  //Camera (shot photo)
  Camera m_camera;

  //Buzzer (Raspberry Pi GPIO)
  Buzzer *m_buzzer;

  //store / load the photo
  QPixmap m_photo;

};
