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

namespace App {
class MainWindow : public QMainWindow {
    Q_OBJECT

    //Displayed widgets
    ViewportWidget *_viewport = nullptr;
    InspectorWidget *_inspector = nullptr;
    HierarchyWidget *_hierarchy = nullptr;

    //Setting up UI and signals
    void setupStatusBar();
    void menuBarSetup();
    void setupToolBar();
    void setupUi();
    void connectSignals();

    //menu_bar and toolbar actions
    QAction *_openAction;
    QAction *_saveAction;
    //status_bar widgets:
    QLabel *_pathLabel;
    QLabel *_resolutionLabel;
    QLabel *_scaleLabel;
    //utils
    EditorSettings _displayOptions;
    size_t _imageIndex = 0;

  public:
    /// @brief Constructs the window and all widgets
    explicit MainWindow(QWidget *parent = nullptr);
    /// @brief Constructs the window with specified size and all widgets in it
    MainWindow(int width, int height, QWidget *parent = nullptr);

  public slots:
    void updatePathLabel(const Image &image);
    void updateScaleLabel(float scale);
    void updateResolutionLabel();
    void updateEditorSettings(const EditorSettings &options);
    /// @brief Acts on imagesChanged() signal from _viewport
    void handleImageUpdate(const std::vector<Image> &images);
    /// @brief Resets transform of currently selected Image
    void resetTransform();
    void openFile();
    void saveFile();
  signals:
    void zoomValueChanged(float value);
    void activeImageChanged(const Image &image);
};
} // namespace App
