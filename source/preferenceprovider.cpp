/* preferenceprovider.cpp
 *
 * Copyright (c) 2018 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'COPYING', which is part of this source code package.
 */
#include "preferenceprovider.h"


auto PreferenceProvider::instance() -> PreferenceProvider&
{
    //thread safe static initializer
    static PreferenceProvider instance;
    return instance;
}

PreferenceProvider::PreferenceProvider(QObject* /*parent*/) : QObject(nullptr)
{
    
}

//Qt Porperty
auto PreferenceProvider::photoFolder() const -> QString
{
    return m_photoFolder;
}

void PreferenceProvider::setPhotoFolder(const QString& i_value)
{
    if (m_photoFolder == i_value) {
        return;
    }

    m_photoFolder = i_value;
    Q_EMIT photoFolderChanged(m_photoFolder);
}

auto PreferenceProvider::photoName() const -> QString
{
    return m_photoName;
}

void PreferenceProvider::setPhotoName(const QString& i_value)
{
    if (m_photoName == i_value) {
        return;
    }

    m_photoName = i_value;
    Q_EMIT photoNameChanged(m_photoName);
}

auto PreferenceProvider::countdown() const -> int
{
    return m_countdown;
}

void PreferenceProvider::setCountdown(const int i_value)
{
    if (m_countdown == i_value) {
        return;
    }

    m_countdown = i_value;
    Q_EMIT countdownChanged(m_countdown);
}

auto PreferenceProvider::countdownColor() const -> QString
{
    return m_countdownColor;
}

void PreferenceProvider::setCountdownColor(const QString& i_value)
{
    if (m_countdownColor == i_value) {
        return;
    }

    m_countdownColor = i_value;
    Q_EMIT countdownColorChanged(m_countdownColor);
}

auto PreferenceProvider::backgroundColor() const -> QString
{
    return m_backgroundColor;
}

void PreferenceProvider::setBackgroundColor(const QString& i_value)
{
    if (m_backgroundColor == i_value) {
        return;
    }

    m_backgroundColor = i_value;
    Q_EMIT backgroundColorChanged(m_backgroundColor);
}

auto PreferenceProvider::cameraMode() const -> QString
{
    return m_cameraMode;
}

void PreferenceProvider::setCameraMode(const QString& i_value)
{
    if (m_cameraMode == i_value) {
        return;
    }

    m_cameraMode = i_value;
    Q_EMIT cameraModeChanged(m_cameraMode);
}

auto PreferenceProvider::argumentLine() const -> QString
{
    return m_argumentLine;
}

void PreferenceProvider::setArgumentLine(const QString& i_value)
{
    if (m_argumentLine == i_value) {
        return;
    }

    m_argumentLine = i_value;
    Q_EMIT argumentLineChanged(m_argumentLine);
}

auto PreferenceProvider::inputPin() const -> unsigned int
{
    return m_inputPin;
}

void PreferenceProvider::setInputPin(const unsigned int i_value)
{
    if (m_inputPin == i_value) {
        return;
    }

    m_inputPin = i_value;
    Q_EMIT inputPinChanged(m_inputPin);
}

auto PreferenceProvider::outputPin() const -> unsigned int
{
    return m_outputPin;
}

void PreferenceProvider::setOutputPin(const unsigned int i_value)
{
    if (m_outputPin == i_value) {
        return;
    }

    m_outputPin = i_value;
    Q_EMIT outputPinChanged(m_outputPin);
}

auto PreferenceProvider::queryInterval() const -> unsigned int
{
    return m_queryInterval;
}

void PreferenceProvider::setQueryInterval(const unsigned int i_value)
{
    if (m_queryInterval == i_value) {
        return;
    }

    m_queryInterval = i_value;
    Q_EMIT queryIntervalChanged(m_queryInterval);
}

auto PreferenceProvider::timeoutValue() const -> int
{
    return m_timeoutValue;
}

void PreferenceProvider::setTimeoutValue(const int i_value)
{
    if (m_timeoutValue == i_value) {
        return;
    }

    m_timeoutValue = i_value;
    Q_EMIT timeoutValueChanged(m_timeoutValue);
}

auto PreferenceProvider::grayscale() const -> bool
{
    return m_grayscale;
}

void PreferenceProvider::setGrayscale(bool i_value)
{
    if (m_grayscale == i_value) {
        return;
    }

    m_grayscale = i_value;
    Q_EMIT grayscaleChanged(m_grayscale);
}

auto PreferenceProvider::showButtons() const -> bool
{
    return m_showButtons;
}

void PreferenceProvider::setShowButtons(const bool i_value)
{
    if (m_showButtons == i_value) {
        return;
    }

    m_showButtons = i_value;
    Q_EMIT showButtonsChanged(m_showButtons);
}

auto PreferenceProvider::print() const -> bool
{
    return m_print;
}

void PreferenceProvider::setPrint(const bool i_value)
{
    if (m_print == i_value) {
        return;
    }

    m_print = i_value;
    Q_EMIT printChanged(m_print);
}

auto PreferenceProvider::printerName() const -> QString
{
    return m_printerName;
}

void PreferenceProvider::setPrinterName(const QString& i_value)
{
    if (m_printerName == i_value) {
        return;
    }

    m_printerName = i_value;
    Q_EMIT printerNameChanged(m_printerName);
}
