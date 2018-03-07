/* camera.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "camera.h"
#include "preferenceprovider.h"

#include <QDateTime>
#include <QProcess>


Camera::Camera(QObject *parent) : QObject(parent),
  m_process(new QProcess(this))
{

}


Camera::~Camera()
{

}


auto Camera::shootPhoto() -> bool
{
  //File name for the current
  m_lastPhoto = QDateTime::currentDateTime().toString(PreferenceProvider::instance().photoName());

  //Program name and arguments
  const QString command = PreferenceProvider::instance().cameraMode() + QLatin1String(" ") + PreferenceProvider::instance().argumentLine();

  //Start programm with given arguments
  m_process->start(command);

  //convert to milliseconds
  auto milliseconds = 1000 * PreferenceProvider::instance().timeoutValue();

  //check time out and process exit code
  if (!m_process->waitForFinished(milliseconds) || m_process->exitCode() != EXIT_SUCCESS) {
    //error
    return false;
  }

  return true;
}
