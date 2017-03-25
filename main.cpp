/* main.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#include "fotobox.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    // qApp
    QApplication app(argc, argv);
    app.setOrganizationName("Thomas Kais");
    app.setApplicationName("Fotobox");


    FotoBox fotobox;
    //Fullscreen
    fotobox.setWindowState(Qt::WindowFullScreen);
    fotobox.show();

    //gphoto2 intitalisierung Probleme?
    if(!fotobox.status)
    {
        exit(EXIT_FAILURE);
    }

    //Start EventLoop
    return app.exec();
}
