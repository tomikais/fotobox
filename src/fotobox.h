/* fotobox.h
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#ifndef FOTOBOX_H
#define FOTOBOX_H

#pragma once
#include <QDialog>
#include <QThread>

#include "camera.h"

class Buzzer;
class Countdown;
class QKeyEvent;

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
  * \param parent QWidget
  */
  explicit FotoBox(QWidget *parent = nullptr);

  /*!
  * \brief FotoBox destructor
  */
  ~FotoBox() override;


private slots:
  /*!
   * \brief trigger camera to shot a photo and try to show it
   */
  void photo();

  /*!
   * \brief show preference dialog and close dialog
   */
  void preferenceDialog();


private:
  /*!
   * \brief filter key press events
   * \details Enter -> take a photo / Escape -> quit application
   * \param event QKeyEvent
   */
  void keyPressEvent(QKeyEvent *event) override;

  /*!
  * \brief This event handler, for event event, can be reimplemented in a subclass to receive mouse release events for the widget.
  * \details no buttons on UI and left click \sa emit start()
  * \param event QMouseEvent
  */
  void mouseReleaseEvent(QMouseEvent *event) override;

  /*!
   * \brief creates a new Buzzer object
   */
  void buzzer();

  /*!
   * \brief try to load the photo to QPixmap
   * \param i_filePath path to photo
   */
  void loadPhoto(const QString& i_filePath);

  /*!
   * \brief move photo to folder set in preferences
   * \return new file location of the photo
   */
  const QString movePhoto();


  //User Interface
  Ui::FotoBoxDialog *m_ui;

  //Buzzer
  Buzzer *m_buzzer;

  //Countdown
  Countdown *m_countdown;

  //handle Buzzer thread (Raspberry Pi GPIO)
  QThread m_workerThread;

  //Camera (shot photo)
  Camera m_camera;

  //store / load the photo
  QPixmap m_photo;

  //working directory
  const QString m_workingDir;

  //photo directory
  const QString m_photoDir;

  //status bar timout value
  static constexpr int STATUSBAR_MSG_TIMEOUT = 4000;


signals:
  /*!
   * \brief signal: start FotoBox
   */
  void start();

  /*!
   * \brief signal: start query pin with Buzzer class
   */
  void startBuzzer();

};

#endif // FOTOBOX_H
