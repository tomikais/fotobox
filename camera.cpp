/* camera.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "camera.h"

#include <QProcess>
#include <QCoreApplication>

Camera::Camera(QObject* parent) :
  QObject(parent)
{

}


auto Camera::takePicture() -> void
{
  QProcess* gphoto2= new QProcess(this);

  //Program name and arguments
  QString program = "gphoto2 --capture-image-and-download --force-overwrite";

  //Start programm with given arguments
  gphoto2->start(program);
  gphoto2->waitForFinished(-1);
}
