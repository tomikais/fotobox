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


Camera::Camera(QObject *parent) : QObject(parent)
{

}


auto Camera::shootPhoto() -> bool
{
  //File name for the current
  m_currentPhoto = QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HH-mm-ss_")) + PreferenceProvider::instance().photoName();

  //Program name and arguments
  auto argument = PreferenceProvider::instance().argumentLine().arg(QStringLiteral("\"") + m_currentPhoto + QStringLiteral("\""));
  auto command = PreferenceProvider::instance().cameraMode() + QStringLiteral(" ") + argument;

#if defined (Q_OS_WIN)
  //try use Windows 10 Linux Subsystem to call gphoto2
  command = QString("bash.exe -c '%1'").arg(command);
#endif

  //Start programm with given arguments
  QProcess process;
  process.start(command);

  //convert to milliseconds
  auto milliseconds = 1000 * PreferenceProvider::instance().timeoutValue();
  process.waitForFinished(milliseconds);

  //check time out and process exit code
  return (process.exitCode() == EXIT_SUCCESS);
}


auto Camera::currentPhoto() const -> QString
{
  return m_currentPhoto;
}
