//
// Created by Volkov Sergey on 16/06/2025.
//

#pragma once
#include <QJsonObject>

namespace App {
struct EditorSettings {
    bool showPath = true;
    bool showResolution = true;
    double zoomValue = 0.1f;
    //Possible to save it persistently using QSettings
    //But I won't do that (I don't want to leave stuff in your system)
};
} //namespace App
