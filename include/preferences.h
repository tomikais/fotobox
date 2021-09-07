/* preferences.h
 *
 * Copyright (c) 2018 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "countdown.h"

#include <QDialog>
#include <QSettings>

namespace Ui {
class PreferencesDialog;
} //namespace Ui

namespace FotoBox {

/*!
 * \brief Preference class to change and store the application settings
 */
class Preferences : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Preferences constructor
     * \param parent QWidget
     */
    explicit Preferences(QWidget *parent = nullptr);

    /*!
     * \brief Preferences destructor
     */
    ~Preferences() override;

    /*!
     * \brief Preferences default copy constructor
     */
    Preferences(const Preferences &other) = delete;

    /*!
     * \brief Preferences default copy assignment
     */
    Preferences &operator=(const Preferences &other) = delete;

    /*!
     * \brief Preferences default move constructor
     */
    Preferences(Preferences &&other) = delete;

    /*!
     * \brief Preferences default move assignment
     */
    Preferences &operator=(Preferences &&other) = delete;

private Q_SLOTS:
    /*!
     * \brief Start the FotoBox
     */
    void startFotoBox();

    /*!
     * \brief Open a color picker to choose background color for FotoBox
     */
    void colorDialog();

    /*!
     * \brief Show help dialog for gphoto2/raspistill command line options
     * \details official documentation and usefull examples
     */
    void commandLineOptionsDialog();

    /*!
     * \brief Open a QFileDialog to choose the photo directory
     */
    void chooseDirectory();

    /*!
     * \brief Clear directory content of the photo directory.
     */
    void clearDirectoryContent();

    /*!
     * \brief Restore default preferences.
     */
    void restoreDefaultPreferences();

    /*!
     * \brief Show the given color in QLineEdit
     * \param i_colorName QString hex name of the color
     */
    void showColor(const QString &i_colorName);

    /*!
     * \brief Check camera application availability and provide help if needed
     * \details gphoto2: set version and camera model
     * \param i_name QString name of the application to check
     */
    void verifyApplication(const QString &i_name);

    /*!
     * \brief Check if the path is useable
     * \param i_path QString directory which stores the photos
     * \return true: path is okay false: something wrong, show warning
     */
    bool verifyPath(const QString &i_path);

private:
    /*!
     * \brief Set the window position
     * \details if the height of the Preference dialog is greater than the available screen height, start maximizing otherwise center the dialog
     */
    void windowPosition();

    /*!
     * \brief Check if pigpio deamon is reachable
     */
    void pigpioDeamon();

    /*!
     * \brief Stop countdown, stop mouse tracking and set normal window title name.
     */
    void stopCountdownMode();

    /*!
     * \brief Signal & Slot connect
     */
    void connectUi();

    /*!
     * \brief Load application settings from INI file
     */
    void loadPreferences();

    /*!
     * \brief Set icons for all QToolButtons
     */
    void setButtonIcons();

    /*!
     * \brief Save the preferences to QSettings
     */
    void savePreferences();

    /*!
     * \brief This event handler, for event event, can be reimplemented in a subclass to receive mouse move events for the widget
     * \details stopping auto accept dialog \see autoAcceptDialog()
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /*!
     * \brief Check if application is installed and available
     * \param i_name QString name of the application to check
     * \param i_message QString error message to display in QLabel
     * \return bool true: application found / false: not found
     */
    auto applicationAvailable(const QString &i_name, const QString &i_message) -> bool;

    /*!
     * \brief Read gphoto2 / libgphoto2 version and camera model
     * \param i_name QString name of the application to check
     * \return QString return information
     */
    auto gphotoInfo(const QString &i_name) -> QString;

    /*! Countdown start value (10 seconds) */
    static constexpr int COUNTDOWN_START_VALUE = 10;

    /*! Default countdown start value (3 seconds) */
    static constexpr int DEFAULT_COUNTDOWN_START_VALUE = 3;

    /*! Default input PIN */
    static constexpr unsigned int DEFAULT_INPUT_PIN = 24;

    /*! Default output PIN */
    static constexpr unsigned int DEFAULT_OUTPUT_PIN = 17;

    /*! Default query interval */
    static constexpr unsigned int DEFAULT_QUERY_INTERVAL = 10;

    /*! Default time out (QProcess timed out) */
    static constexpr int DEFAULT_TIMEOUT = 30;

    /*! User Interface */
    Ui::PreferencesDialog *m_ui;

    /*! Store and read settings (INI file) */
    QSettings m_settings;

    /*! Countdown to "auto close dialog" */
    Countdown m_countdown;
};

} // end namespace FotoBox

#endif // PREFERENCES_H
