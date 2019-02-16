/* camera.cpp
 *
 * Copyright (c) 2018 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "camera.h"
#include "preferenceprovider.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QProcess>


Camera::Camera(QObject *parent) : QObject(parent),
  m_photoSuffix(PreferenceProvider::instance().photoName()),
  m_cameraMode(PreferenceProvider::instance().cameraMode()),
  m_argLine(PreferenceProvider::instance().argumentLine()),
  m_timeoutValue(1000 * PreferenceProvider::instance().timeoutValue()),
  m_process(new QProcess(this))
{

}


bool Camera::shootPhoto()
{
  //File name for the current
  m_currentPhoto = QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HH-mm-ss_")) + m_photoSuffix;

  //Program name and arguments
  auto command = m_cameraMode + QStringLiteral(" ") + m_argLine.arg(QStringLiteral("\"") + m_currentPhoto + QStringLiteral("\""));

#if defined (Q_OS_WIN)
  //try use Windows 10 Linux Subsystem to call gphoto2
  command = QString("bash.exe -c '%1'").arg(command);
#endif

  //Start programm with given arguments
  m_process->start(command);
  m_process->waitForFinished(m_timeoutValue);

  //check time out and process exit code
  return (m_process->exitCode() == EXIT_SUCCESS);
}


QString Camera::currentPhoto() const
{
  return m_currentPhoto;
}
