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
  QProcess* gphoto2= new QProcess(this);

  //Program name and arguments
  const QString program = "gphoto2 --capture-image-and-download --keep --force-overwrite";

  //Start programm with given arguments
  gphoto2->start(program);

  //timout 15secs = 15000msecs
  const int msecs = 15000;
  return gphoto2->waitForFinished(msecs);
}
