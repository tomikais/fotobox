/* camera.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once
#include <QObject>

class QProcess;

/*!
 * \brief The Camera class
 * Use gphoto2 to send commands to camera.
 */
class Camera : public QObject
{
  Q_OBJECT

public:
  /*!
   * \brief Camera constructor
   * \param parent QObject
   */
  explicit Camera(QObject *parent = nullptr);

  /*!
    * \brief Camera destructor
    */
  ~Camera();

  /*!
   * \brief takePicture
   * \details
   * \return const bool true: processed finished
   */
  auto takePicture() const -> bool;

private:
  //start gphoto2 (external program)
  QProcess *m_process;

};
