//
// Created by Volkov Sergey on 16/06/2025.
//

#pragma once
#include "hierarchy_widget.hpp"
#include "inspector_widget.hpp"
#include "viewport_widget.hpp"

#include <QAction>
#include <QLabel>
#include <QMainWindow>
#include <QSettings>

namespace App {
class MainWindow : public QMainWindow {
    Q_OBJECT

    //Displayed widgets
    ViewportWidget *_viewport = nullptr;
    InspectorWidget *_inspector = nullptr;
    HierarchyWidget *_hierarchy = nullptr;

    void setupUi();
    void connectSignals();

    //menu_bar actions
    QAction *_openAction;
    //status_bar widgets:
    QLabel *_pathLabel;
    QLabel *_resolutionLabel;
    QLabel *_scaleLabel;
    //utils
    DisplayOptions _displayOptions;
    size_t imageIndex = 0;

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(int width, int height, QWidget *parent = nullptr);

  public slots:
    void updatePathLabel(const Image &image);
    void updateScaleLabel(float scale);
    void updateResolutionLabel();
    void updateDisplayOptions(const DisplayOptions &options);
    void handleImageUpdate(const std::vector<Image> &images);
  signals:
    void zoomValueChanged(float value);
    void activeImageChanged(const Image &image);
};
} // namespace App
