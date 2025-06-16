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

class Image {
    //Base information
    QFileInfo _fileInfo;
    QString _name;
    QImage _img; //probably not necessary to save

    static QTransform transformFromJson(const QJsonObject &json);
  public:
    //Image parameters
    QTransform transform;
    float opacity = 1;
    float zValue = 0;
    // bool showBorder = false;
    // QColor borderColor = Qt::black;
    bool isVisible = true;

    Image(const QString &path);
    Image(const QJsonObject &json);

    QFileInfo getFileInfo() const { return _fileInfo; }
    QString getName() const { return _name; }
    QImage getImage() const { return _img; }
    QJsonObject toJson() const;
    QJsonObject transformToJson() const;
};


} // namespace App
