/* camera.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "camera.h"

#include <QProcess>


Camera::Camera(QObject* parent) :
  QObject(parent),
  m_process(new QProcess(this))
{
  //check pointer
  Q_CHECK_PTR(m_process);
}


Camera::~Camera()
{
  //clean up
  m_process->deleteLater();
}

//TODO use setting to handle options for gphoto2
auto Camera::takePicture() const -> const bool
{
  //Program name and arguments
  const QString gphoto2 = "gphoto2 --capture-image-and-download --keep --force-overwrite";

  //Start programm with given arguments
  m_process->start(gphoto2, QIODevice::NotOpen);

  //start call and check if everthing was okay
  if(!m_process->waitForFinished(m_msecs) || m_process->exitCode() != EXIT_SUCCESS){
    //error
#ifdef QT_NO_DEBUG //in debug return true
    return false;
#endif
  }

  return true;
}
