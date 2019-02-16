/* main.cpp
 *
 * Copyright (c) 2018 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "preferences.h"

#include <QApplication>
#include <QLibraryInfo>
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
  QApplication::setOrganizationName(QStringLiteral("Thomas Kais"));
  QApplication::setApplicationName(QStringLiteral("FotoBox"));
  QApplication::setApplicationVersion(QStringLiteral("1.2.7"));

  QTranslator qtTranslator, appTranslator;
  // Qt Translation
  if (qtTranslator.load(QLocale(), QLatin1String("qt"), QLatin1String("_"), QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
      QApplication::installTranslator(&qtTranslator);
    }
  //App Translation: German or English (=default language)
  bool result = false;
  if (QLocale::system().language() == QLocale::German) {
      result = appTranslator.load(QStringLiteral(":/i18n/translation_de.qm"));
    }
  else {
      result = appTranslator.load(QStringLiteral(":/i18n/translation_en.qm"));
    }
  if (result) {
      QApplication::installTranslator(&appTranslator);
    }

  //Show preferences dialog
  auto* dialog = new Preferences;
  dialog->show();

  return QApplication::exec();
}
