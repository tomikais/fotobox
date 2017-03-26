/* fotobox.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once
#include <QMainWindow>
#include <QGraphicsScene>
#include <QProcess>

class Settings;
class Buzzer;

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
    explicit FotoBox(QWidget* parent = nullptr);
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
    const int countdown_finished;
    int countdown_time;
    QProcess* gphoto2;
    Buzzer* m_workerThread;
};
