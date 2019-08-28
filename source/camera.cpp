/* camera.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#include "camera.h"

#include "preferenceprovider.h"

#include <QCoreApplication>
#include <QDateTime>

Camera::Camera(QObject *parent)
    : QObject(parent)
    , m_timeoutValue(1000 * PreferenceProvider::instance().timeoutValue())
    , m_photoSuffix(PreferenceProvider::instance().photoName())
    , m_cameraMode(PreferenceProvider::instance().cameraMode().append(' '))
    , m_argLine(PreferenceProvider::instance().argumentLine().arg(QStringLiteral("\"%1\"")))
    , m_process(this)
{
    //call this function to save memory, if you are not interested in the output of the process
    m_process.closeReadChannel(QProcess::StandardOutput);
    m_process.closeReadChannel(QProcess::StandardError);

    //use 'photo folder' as working dir
    m_process.setWorkingDirectory(PreferenceProvider::instance().photoFolder());
}

bool Camera::shootPhoto()
{
    //file name for the current
    m_currentPhoto = QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HH-mm-ss_")) + m_photoSuffix;

    //program name and arguments
    auto command = m_cameraMode + m_argLine.arg(m_currentPhoto);

#if defined(Q_OS_WIN)
    //try use Windows 10 Linux Subsystem to call gphoto2
    command = QString(QStringLiteral("bash.exe -c '%1'")).arg(command);
#endif

    //start programm with given arguments
    m_process.start(command);
    m_process.waitForFinished(m_timeoutValue);

    //check time out and process exit code
    return (m_process.exitCode() == EXIT_SUCCESS);
}

QString Camera::currentPhoto() const
{
    return m_currentPhoto;
}
