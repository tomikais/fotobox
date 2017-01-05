/* fotobox.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#ifndef FOTOBOX_H
#define FOTOBOX_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QProcess>

#include "settings.h"
#include <wiringPi.h>
#include <stdio.h>

#define SECOUND 1000
#define ENDLESS -1

namespace Ui
{
class MainWindow;
}


class FotoBox : public QMainWindow
{
    Q_OBJECT


public:
    //METHODEN
    explicit FotoBox(QWidget* parent = 0);
    ~FotoBox();

    //MEMBER
    bool status;

public slots:
    void startShot();
    void updateCountdown();


protected:


private:
    //METHODEN
    void showResults();

    // MEMBER
    Ui::MainWindow* ui; //User Interface
    Settings* m_settings;
    QTimer* countdown;
    const int countdowntimeout = 3;
    int countdowntime;
    QProcess* gphoto2;
};

#endif // FOTOBOX_H
