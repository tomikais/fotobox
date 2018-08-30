/* main.cpp
 *
 * Copyright (c) 2018 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "preferences.h"

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
  app.setOrganizationName(QStringLiteral("Thomas Kais"));
  app.setApplicationName(QStringLiteral("FotoBox"));
  app.setApplicationVersion(QStringLiteral("1.2.6"));

  //German or English (=default language)
  QTranslator translator;
  bool result = false;
  if (QLocale::system().language() == QLocale::German) {
      result = translator.load(QStringLiteral(":/translations/german"));
    }
  else {
      result = translator.load(QStringLiteral(":/translations/english"));
    }
  if (result) {
      app.installTranslator(&translator);
    }

  //Show preferences dialog
  auto* dialog = new Preferences;
  dialog->show();

  return app.exec();
}
