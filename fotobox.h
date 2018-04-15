/* fotobox.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once
#include <QDialog>

#include "camera.h"

class QKeyEvent;
class Buzzer;

namespace Ui
{
  class FotoBoxDialog;
} //namespace Ui


/*!
 * \brief The FotoBox class
 * Main class to control UI and controll the process
 */
class FotoBox : public QDialog
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
  ~FotoBox() override;


private:
  /*!
   * \brief filter key press events
   * \details Enter -> take a photo / Escape -> quit application
   * \param event QKeyEvent
   */
  auto keyPressEvent(QKeyEvent *event) -> void override;

  /*!
  * \brief This event handler, for event event, can be reimplemented in a subclass to receive mouse release events for the widget.
  * \details no buttons on UI and left click \sa emit start()
  * \param event QMouseEvent
  */
  auto mouseReleaseEvent(QMouseEvent *event) -> void override;

  /*!
   * \brief trigger camera and try to show photo
   */
  auto startProcess() -> void;

  /*!
   * \brief try to load the photo to QPixmap
   * \param i_filePath path to photo
   */
  auto loadPhoto(const QString& i_filePath) -> void;

  /*!
   * \brief move photo to folder set in preferences
   * \return new file location of the photo
   */
  auto movePhoto() -> const QString;


  //User Interface
  Ui::FotoBoxDialog *m_ui;

  //Buzzer (Raspberry Pi GPIO)
  Buzzer *m_buzzer;

  //Camera (shot photo)
  Camera m_camera;

  //store / load the photo
  QPixmap m_photo;

  //status bar timout value
  static int STATUSBAR_MSG_TIMEOUT;


signals:
  /*!
   * \brief Signal: Start shooting a picture
   */
  void start();

};
