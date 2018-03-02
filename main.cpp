/* main.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "preferences.h"
#include "fotobox.h"

#include <QApplication>
#include <QTranslator>


/*!
* \brief main
* \param argc argument count
* \param argv argument vector
* \return int EXIT CODE
*/
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  app.setOrganizationName("Thomas Kais");
  app.setApplicationName("FotoBox");
  app.setApplicationVersion("1.2.0");

  //German or English (=default language)
  QTranslator translator;
  bool result = false;
  if (QLocale::system().language() == QLocale::German) {
    result = translator.load(":/translations/german");
  }
  else {
    result = translator.load(":/translations/english");
  }
  result == true ? app.installTranslator(&translator) : false;

  //Show preferences
  Preferences prefDialog;
  prefDialog.exec();
  if (prefDialog.result() == QDialog::Rejected) {
    return EXIT_FAILURE;
  }

  //Show FotoBox in fullscreen mode
  FotoBox fotobox;
  fotobox.showFullScreen();

  return app.exec();
}

