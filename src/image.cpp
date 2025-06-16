//
// Created by Volkov Sergey on 16/06/2025.
//

#include "image.hpp"
namespace App {

QTransform Image::transformFromJson(const QJsonObject &json) {
    if (!json.contains("m11") || !json.contains("m12") ||
        !json.contains("m13") || !json.contains("m21") ||
        !json.contains("m22") || !json.contains("m23") ||
        !json.contains("m31") || !json.contains("m32") ||
        !json.contains("m33")) {
        throw std::invalid_argument("Image::transformFromJson: Invalid JSON");
    }
    //If there are some issues with values but keys are there I'm assuming defaults
    return QTransform(json["m11"].toDouble(1.0), json["m12"].toDouble(0.0),
                      json["m13"].toDouble(0.0), json["m21"].toDouble(0.0),
                      json["m22"].toDouble(1.0), json["m23"].toDouble(0.0),
                      json["m31"].toDouble(0.0), json["m32"].toDouble(0.0),
                      json["m33"].toDouble(1.0));
}
Image::Image(const QString &path)
    : _fileInfo(path), _name(_fileInfo.completeBaseName()),
      _img(QImage(_fileInfo.absoluteFilePath())) {
    if (!_fileInfo.exists() || _img.isNull()) {
        throw std::invalid_argument(
            std::format("Image.hpp: Failed to open file {}",
                        _fileInfo.filesystemAbsoluteFilePath().string()));
    }
}
Image::Image(const QJsonObject &json) : Image(json["path"].toString()) {
    if (!json.contains("transform") || !json.contains("opacity")
    || !json.contains("zValue") || !json.contains("isVisible")) {
        throw std::invalid_argument("Image::Image(QJsonObject): Invalid JSON");
    }
    transform = transformFromJson(json["transform"].toObject());
    opacity = json["opacity"].toDouble(1.0f);
    zValue = json["zValue"].toDouble(0);
    isVisible = json["isVisible"].toBool(true);

}
QJsonObject Image::toJson() const {
    QJsonObject result;
    result["path"] = _fileInfo.absoluteFilePath();
    result["transform"] = transformToJson();
    result["opacity"] = opacity;
    result["zValue"] = zValue;
    result["isVisible"] = isVisible;
    return result;
}
QJsonObject Image::transformToJson() const {
    QJsonObject result;
    result["m11"] = transform.m11();
    result["m12"] = transform.m12();
    result["m13"] = transform.m13();
    result["m21"] = transform.m21();
    result["m22"] = transform.m22();
    result["m23"] = transform.m23();
    result["m31"] = transform.m31();
    result["m32"] = transform.m32();
    result["m33"] = transform.m33();
    return result;

}

} // namespace App