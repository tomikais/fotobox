/* camera.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QProcess>

/*!
 * \brief The Camera class
 * Use gphoto2 to send commands to camera.
 */
class Camera : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief Camera constructor
     * \param parent QObjec
     */
    explicit Camera(QObject *parent = nullptr);

    /*!
     * \brief Camera destructor
     */
    ~Camera() override = default;

    /*!
     * \brief Camera default copy constructor
     */
    Camera(const Camera &other) = delete;

    /*!
     * \brief Camera default copy assignment
     */
    Camera &operator=(const Camera &other) = delete;

    /*!
     * \brief Camera default move constructor
     */
    Camera(Camera &&other) = delete;

    /*!
     * \brief Camera default move assignment
     */
    Camera &operator=(Camera &&other) = delete;

    /*!
     * \brief shoot a photo with gphoto2
     * \return true: processed finished / false: error
     */
    bool shootPhoto();

    /*!
     * \brief return member \see m_currentPhoto
     * \return filepath current photo
     */
    QString currentPhoto() const;

private:
    /*! used to convert msec to sec */
    static constexpr int TO_SECONDS = 1000;

    /*! time out value */
    const int m_timeoutValue;

    /*! name of the current photo */
    QString m_currentPhoto;

    /*! name of the photo */
    const QString m_photoSuffix;

    /*! camera mode */
    const QString m_cameraMode;

    /*! list of command line arguments */
    const QStringList m_argList;

    /*! executing commands */
    QProcess m_process;
};

#endif // CAMERA_H
