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

class QKeyEvent;
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
  virtual ~FotoBox();

  /*!
   * \brief check if gphoto2 is installed on system
   * \return true: gphoto2 available / false: not
   */
  auto static checkGPhoto2() -> bool;


private:
  /*!
   * \brief filter key press events
   * \details Enter -> take a photo / Escape -> quit application
   * \param event QKeyEvent
   */
  auto virtual keyPressEvent(QKeyEvent *event) -> void override;

  /*!
   * \brief show the picture that was taken
   */
  auto showPicture() -> void;


  //User Interface
  Ui::Fotobox *m_ui;

  //Buzzer (Raspberry Pi GPIO)
  Buzzer *m_buzzer;

  //Camera (shot photo)
  Camera m_camera;

  //store / load the photo
  QPixmap m_photo;


signals:
  /*!
   * \brief Signal: Start shooting a picture
   */
  void start();

};
