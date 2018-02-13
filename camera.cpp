/* camera.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "camera.h"
#include "preferences.h"

#include <QProcess>


Camera::Camera(QObject *parent) : QObject(parent),
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
auto Camera::takePicture() const -> bool
{
  //Program name and arguments
  const QString gphoto2 = QStringLiteral("gphoto2 ") + Preferences::getInstance().argumentLine();

  //Start programm with given arguments
  m_process->start(gphoto2, QIODevice::NotOpen);

  //convert to milliseconds
  auto milliseconds = 1000 * Preferences::getInstance().timeoutValue();

  //start call and check if everthing was okay
  if(!m_process->waitForFinished(milliseconds) || m_process->exitCode() != EXIT_SUCCESS){
      //error
      return false;
    }

  return true;
}
