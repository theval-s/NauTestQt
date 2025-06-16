//
// Created by Volkov Sergey on 15/06/2025.
//

#pragma once
#include <QFileInfo>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QString>
#include <QJsonObject>

namespace App {

/// @class Image
/// @brief Represents image, with display data and transformation
class Image {
    //Base information
    QFileInfo _fileInfo;
    QString _name; ///based on _fileInfo
    QImage _img; //probably not necessary to save

    /// @brief Constructs QTransform from JSON
    static QTransform transformFromJson(const QJsonObject &json);
  public:
    //Image parameters
    QTransform transform;
    float opacity = 1;
    float zValue = 0;
    bool isVisible = true;

    /// @brief Constructs Image from filepath
    /// @param path The path to image file
    Image(const QString &path);
    /// @brief Constructs Image from JSON object
    /// @param json JSON object with Image data
    Image(const QJsonObject &json);

    QFileInfo getFileInfo() const { return _fileInfo; }
    QString getName() const { return _name; }
    QImage getImage() const { return _img; }
    QJsonObject toJson() const;
    /// @brief Serializes QTransform of the Image to JSON
    QJsonObject transformToJson() const;
};


} // namespace App
