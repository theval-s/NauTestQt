//
// Created by Volkov Sergey on 16/06/2025.
//

#pragma once
#include "viewport_widget.hpp"

#include <QLabel>
#include <QMainWindow>
#include <QAction>

namespace App {
class MainWindow : public QMainWindow {
    Q_OBJECT

    ViewportWidget *_viewport = nullptr;

    void setupUi();
    void connectSignals();


    //menu_bar actions
    QAction *_openAction;
    //status_bar widgets:
    QLabel *_pathLabel;
    QLabel *_resolutionLabel;
    QLabel *_scaleLabel;

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    MainWindow(int width, int height, QWidget *parent = nullptr);

  public slots:
    void updatePathLabel();
    void updateScaleLabel(float scale);
    void updateResolutionLabel();

};
} // namespace App
