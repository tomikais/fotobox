/* camera.h
 *
 * Copyright (c) 2017 Thomas Kais
 *
 * This file is subject to the terms and conditions defined in
 * file 'LICENSE', which is part of this source code package.
 */
#pragma once
#include <QObject>

class Camera : public QObject
{
  Q_OBJECT
public:
  explicit Camera(QObject *parent = 0);

  auto takePicture() const -> void

signals:

public slots:

};
