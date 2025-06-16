//
// Created by Volkov Sergey on 16/06/2025.
//

#pragma once
#include "editor_settings.hpp"
#include "image.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

namespace App {

class SavingManager {
  public:
    static void saveProject(const std::vector<Image> &images, QString filePath);
    static std::vector<Image> loadProject(QString filePath);

    //saveEditorSettings/loadEditorSettings not implemented,
    //(see ../include/editor_settings.hpp)
};
//Json example serialization:
/*
{
    "images": [
        {
            "isVisible": true,
            "opacity": 0.5199999809265137,
            "path": "/Users/val/Pictures/finalspace2.png",
            "transform": {
                "m11": 0.9981347984218669,
                "m12": 0.06104853953485687,
                "m13": 0,
                "m21": -0.06104853953485687,
                "m22": 0.9981347984218669,
                "m23": 0,
                "m31": 0,
                "m32": 0,
                "m33": 1
            },
            "zValue": 0
        }
    ]
}
*/
} // namespace App
