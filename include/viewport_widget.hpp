//
// Created by Volkov Sergey on 12/06/2025.
//

#pragma once
#include "image.hpp"

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QWheelEvent>

namespace App {
// TODO: Add ImagesChanged signal and slot
constexpr float ZOOM_VALUE = 0.1f; // get from settings later
constexpr float ZOOM_MIN = 0.1f;
constexpr float ZOOM_MAX = 100.0f;

class ViewportWidget : public QGraphicsView {
    Q_OBJECT

    float _currentScale = 1.0f;
    QGraphicsScene _scene;
    std::vector<std::pair<Image, QGraphicsPixmapItem *>> _items;
    qreal maxHeight = 0, maxWidth = 0;

    void wheelEvent(QWheelEvent *event) override;

  public:
    explicit ViewportWidget(QWidget *parent = nullptr);
    // ~ViewportWidget();
    void setImage(const Image &image);
    void addImage(const Image &image);
    float getScale() const { return _currentScale; }
    void setScale(float scale);
    [[nodiscard]] std::vector<Image> getImages() const;
    void updateSceneRect();
    void applyParametersToPixmapItem(const Image& img, size_t pixmap_ind);
};

} // namespace App
