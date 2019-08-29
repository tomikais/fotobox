/* fotobox.h
 *
 * Copyright (c) 2016 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#ifndef FOTOBOX_H
#define FOTOBOX_H

#include "buzzer.h"
#include "camera.h"
#include "countdown.h"

#include <QDialog>
#include <QThread>

class QKeyEvent;

namespace Ui {
class FotoBoxDialog;
} //namespace Ui

/*!
 * \brief The FotoBox class
 * Main class to control UI and controll the process
 */
class FotoBox : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief FotoBox constructor
     * \param parent QWidget
     */
    explicit FotoBox(QWidget *parent = nullptr);

    /*!
     * \brief FotoBox destructor
     */
    ~FotoBox() override;

    /*!
     * \brief FotoBox default copy constructor
     */
    FotoBox(const FotoBox &other) = delete;

    /*!
     * \brief FotoBox default copy assignment
     */
    FotoBox &operator=(const FotoBox &other) = delete;

    /*!
     * \brief FotoBox default move constructor
     */
    FotoBox(FotoBox &&other) = delete;

    /*!
     * \brief FotoBox default move assignment
     */
    FotoBox &operator=(FotoBox &&other) = delete;

private Q_SLOTS:
    /*!
     * \brief trigger camera to shot a photo and try to show it
     */
    void photo();

    /*!
     * \brief show preference dialog and close dialog
     */
    void preferenceDialog();

private:
#if defined(Q_OS_MACOS)
    /*!
     * \brief Is used as a workaround for macOS to close a fullscreen window.
     * \details close the window again, using the native MacOS API (https://bugreports.qt.io/browse/QTBUG-46701)
     */
    void closeFullscreenWindowOnMac();
#endif

    /*!
     * \brief filter key press events
     * \details Enter -> take a photo / Escape -> quit application
     * \param event QKeyEvent
     */
    void keyPressEvent(QKeyEvent *event) override;

    /*!
     * \brief This event handler, for event event, can be reimplemented in a subclass to receive mouse release events for the widget.
     * \details no buttons on UI and left click \see emit start()
     * \param event QMouseEvent
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /*!
     * \brief With or without buttons
     */
    void buttons();

    /*!
     * \brief With or without countdown
     */
    void countdown();

    /*!
     * \brief creates a new Buzzer object
     */
    void buzzer();

    /*!
     * \brief try to load the photo to QPixmap
     * \param i_filePath path to photo
     */
    void loadPhoto(const QString &i_filePath);

    /*!
     * \brief Draw text on the QPixmap \see m_photo
     * \param i_text text to write
     */
    void drawText(const QString &i_text);

    /*!
     * \brief calculate "maximum" fitting font size
     * \param i_width double
     * \param i_widthFont double
     * \return best fitting fontsize
     */
    double calculateFontSize(double i_width, double i_widthFont);

    /*! User Interface */
    Ui::FotoBoxDialog *m_ui;

    /*! Countdown */
    Countdown m_countdown;

    /*! handle Buzzer thread (Raspberry Pi GPIO) */
    QThread m_workerThread;

    /*! buzzer using wiringPi framework */
    Buzzer m_buzzer;

    /*! Camera (shot photo) */
    Camera m_camera;

    /*! store / load the photo */
    QPixmap m_photo;

    /*! photo directory */
    const QString m_photoDir;

    /*! status bar timout value */
    static constexpr int STATUSBAR_MSG_TIMEOUT = 8000;

    /*! keyboard shortcut to start FotoBox */
    const std::vector<Qt::Key>
        m_triggerKey{Qt::Key_N, Qt::Key_Return, Qt::Key_Enter, Qt::Key_PageDown, Qt::Key_PageUp, Qt::Key_Left, Qt::Key_Right, Qt::Key_Up, Qt::Key_Down, Qt::Key_Space, Qt::Key_Backspace};

    /*! keyboard shortcut to go back to preference dialog */
    const std::vector<Qt::Key> m_preferenceKey{Qt::Key_P, Qt::Key_S, Qt::Key_E};

    /*! keyboard shortcut to quit the application */
    const std::vector<Qt::Key> m_quitKey{Qt::Key_Escape, Qt::Key_Q};

Q_SIGNALS:
    /*!
     * \brief signal: start FotoBox
     */
    void start();

    /*!
     * \brief signal: start query pin with Buzzer class
     */
    void startBuzzer();
};

#endif // FOTOBOX_H
