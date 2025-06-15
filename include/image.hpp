//
// Created by Volkov Sergey on 15/06/2025.
//

#pragma once
#include <QFileInfo>
#include <QImage>
#include <QString>
#include <QGraphicsPixmapItem>

namespace App {

class Image {
    //Base information
    QFileInfo _fileInfo;
    QString _name;
    QImage _img;

  public:
    //Image parameters
    QTransform _transform;
    float _rotation;
    float _opacity;
    bool _showBorder = false;
    QColor _borderColor = Qt::black;
    bool _isVisible = true;

    explicit Image(const QFileInfo &path)
        : _fileInfo(path), _name(_fileInfo.completeBaseName()),
          _img(QImage(_fileInfo.absoluteFilePath())) {
        if (!_fileInfo.exists() || _img.isNull()) {
            throw std::invalid_argument(std::format(
                "Image.hpp: Failed to open file {}", _fileInfo.absoluteFilePath()));
        }
    }

};

} // namespace App
