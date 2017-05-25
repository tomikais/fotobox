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
  QObject(parent)
{

}


//TODO use setting to handle options for gphoto2
auto Camera::takePicture() -> const bool
{
  //start gphoto2 (external program)
  auto process = new QProcess(this, QIODevice::NotOpen);

  //Program name and arguments
  const QString gphoto2 = "gphoto2 --capture-image-and-download --keep --force-overwrite";

  //Start programm with given arguments
  process->start(gphoto2);

  //timout 15secs = 15000msecs
  const int msecs = 15000;

  //start call and check if everthing was okay
  if(!process->waitForFinished(msecs) || process->exitCode() != EXIT_SUCCESS){
    //error
    return false;
  }

  return true;
}
