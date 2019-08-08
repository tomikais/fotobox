/* main.cpp
 *
 * Copyright (c) 2019 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#include "preferences.h"

#include <QApplication>
#include <QLibraryInfo>
#include <QSplashScreen>
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
    QApplication::setApplicationVersion(QStringLiteral("1.3.4"));

    //splash screen FotoBox logo
    QPixmap pixmap(QStringLiteral(":/resources/logo"));
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    splash.show();

    QTranslator qtTranslator, appTranslator;
    //Qt Translation
    if (qtTranslator.load(QLocale(), QStringLiteral("qt"), QStringLiteral("_"), QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        QApplication::installTranslator(&qtTranslator);
    }
    //app Translation: German or English (=default language)
    bool result = false;
    if (QLocale::system().language() == QLocale::German) {
        result = appTranslator.load(QStringLiteral(":/resources/translation_de"));
    } else {
        result = appTranslator.load(QStringLiteral(":/resources/translation_en"));
    }
    if (result) {
        QApplication::installTranslator(&appTranslator);
    }

    //show preferences dialog and close splashscreen
    auto *dialog = new Preferences;
    splash.finish(dialog);
    dialog->show();

    return QApplication::exec();
}
