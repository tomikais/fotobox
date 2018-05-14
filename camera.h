/* camera.h
 *
 * Copyright (c) 2018 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once
#include <QObject>


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
   ~Camera() = default;

  /*!
   * \brief shoot a photo with gphoto2
   * \return true: processed finished / false:error
   */
  auto shootPhoto() -> bool;

  /*!
   * \brief return member \sa m_currentPhoto
   * \return filepath current photo
   */
  auto currentPhoto() const -> QString;


private:
  //name of the current photo
  QString m_currentPhoto;

};
