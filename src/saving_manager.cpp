//
// Created by Volkov Sergey on 16/06/2025.
//

#include "saving_manager.hpp"

namespace App {

void SavingManager::saveProject(const std::vector<Image> &images,
                                QString filePath) {
    QJsonObject json;
    QJsonArray images_array;
    for (const auto &image : images) {
        images_array.append(image.toJson());
    }
    json["images"] = images_array;

    QFile file(filePath);
    if (!file.open(QFile::WriteOnly)) {
        throw std::runtime_error("Could not open file for writing");
    }
    QJsonDocument doc = QJsonDocument(json);
    file.write(doc.toJson());
}
std::vector<Image> SavingManager::loadProject(QString filePath) {
    if (!filePath.endsWith(".json"))
        throw std::invalid_argument("File is not .json");
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly)) {
        throw std::runtime_error("Could not open file for reading");
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isEmpty())
        throw std::runtime_error(
            "Could not parse file (it might be empty or corrupted)");

    QJsonObject json = doc.object();
    if (!json.contains("images")) throw std::invalid_argument("Invalid JSON");
    std::vector<Image> images;
    QJsonArray images_array = json["images"].toArray();
    for (const auto &image : images_array) {
        Image img(image.toObject());
        images.push_back(img);
    }
    return images;
}
} // namespace App
