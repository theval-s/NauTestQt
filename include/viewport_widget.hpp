//
// Created by Volkov Sergey on 12/06/2025.
//

#pragma once
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QWheelEvent>

#include "image.hpp"


namespace App {
constexpr float ZOOM_VALUE = 0.1f; // get from settings later
constexpr float ZOOM_MIN = 0.1f;
constexpr float ZOOM_MAX = 100.0f;

class ViewportWidget : public QGraphicsView {
    Q_OBJECT

    float _currentScale = 1.0f;
    QGraphicsScene _scene;
    std::vector<std::pair<Image, QGraphicsPixmapItem *>> _items;

    void wheelEvent(QWheelEvent *event) override;

  public:
    explicit ViewportWidget(QWidget *parent = nullptr);
    // ~ViewportWidget();
    void setImage(const Image &image);
    void addImage(const Image &image);
    float getScale() const { return _currentScale; }
    void setScale(float scale);
};

} // namespace App
