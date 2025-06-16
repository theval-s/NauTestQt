//
// Created by Volkov Sergey on 15/06/2025.
//

#pragma once
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QString>

namespace App {

class Image {
    //Base information
    QFileInfo _fileInfo;
    QString _name;
    QImage _img;

  public:
    //Image parameters
    QTransform transform;
    float opacity = 1;
    float zValue = 0;
    // bool showBorder = false;
    // QColor borderColor = Qt::black;
    bool isVisible = true;

    Image(const QString &path)
        : _fileInfo(path), _name(_fileInfo.completeBaseName()),
          _img(QImage(_fileInfo.absoluteFilePath())) {
        if (!_fileInfo.exists() || _img.isNull()) {
            throw std::invalid_argument(
                std::format("Image.hpp: Failed to open file {}",
                            _fileInfo.filesystemAbsoluteFilePath().string()));
        }
    }

    QFileInfo getFileInfo() const { return _fileInfo; }
    QString getName() const { return _name; }
    QImage getImage() const { return _img; }
};

} // namespace App
