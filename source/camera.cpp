/* camera.cpp
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#include "camera.h"

#include "preferenceprovider.h"

#include <QDateTime>

namespace FotoBox {

Camera::Camera(QObject *parent)
    : QObject(parent)
    , m_timeoutValue(TO_SECONDS * PreferenceProvider::instance().timeoutValue())
    , m_photoSuffix(PreferenceProvider::instance().photoName())
    , m_cameraMode(PreferenceProvider::instance().cameraMode())
    , m_argList(PreferenceProvider::instance().argumentLine().split(QChar(' ')))
    , m_process(this)
{
    //call this function to save memory, if you are not interested in the output of the process
    m_process.closeReadChannel(QProcess::StandardOutput);
    m_process.closeReadChannel(QProcess::StandardError);

    //use 'photo folder' as working dir
    m_process.setWorkingDirectory(PreferenceProvider::instance().photoFolder());
}

auto Camera::shootPhoto() -> bool
{
    //file name of photo
    m_currentPhoto = QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HH-mm-ss_")) + m_photoSuffix;
    //add filename to argument
    auto arguments = m_argList;
    arguments = arguments.replaceInStrings(QStringLiteral("%1"), m_currentPhoto);

    //start programm with given arguments
    m_process.start(m_cameraMode, arguments, {});
    m_process.waitForFinished(m_timeoutValue);

    //check time out and process exit code
    return (m_process.exitCode() == EXIT_SUCCESS);
}

auto Camera::currentPhoto() const -> QString
{
    return m_currentPhoto;
}

} // end namespace FotoBox
