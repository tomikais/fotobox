/* camera.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "camera.h"
#include "preferenceprovider.h"

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


auto Camera::shootPhoto() const -> bool
{
  //Program name and arguments
  const QString command = PreferenceProvider::instance().cameraMode() + " " + PreferenceProvider::instance().argumentLine();

  //Start programm with given arguments
  m_process->start(command, QIODevice::NotOpen);

  //convert to milliseconds
  auto milliseconds = 1000 * PreferenceProvider::instance().timeoutValue();

  //check time out and process exit code
  if (!m_process->waitForFinished(milliseconds) || m_process->exitCode() != EXIT_SUCCESS) {
    //error
    return false;
  }

  return true;
}
