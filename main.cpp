/* main.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "fotobox.h"

#include <QApplication>


/*!
 * \brief main
 * \param argc argument count
 * \param argv argument vector
 * \return int EXIT CODE
 */
int main(int argc, char *argv[])
{
  //Set Qt application settings
  QApplication app(argc, argv);
  app.setOrganizationName("Thomas Kais");
  app.setApplicationName("Fotobox");
  app.setApplicationVersion("1.0.0");

  FotoBox fotobox;
  //Fullscreen
  fotobox.setWindowState(Qt::WindowFullScreen);
  fotobox.show();

  //Start the Qt EventLoop
  return app.exec();
}
