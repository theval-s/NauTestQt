//
// Created by Volkov Sergey on 12/06/2025.
//

#include "viewport_widget.hpp"

namespace App {

void ViewportWidget::wheelEvent(QWheelEvent *event) {
    const float scaleValue =
        event->angleDelta().y() > 0 ? 1 + ZOOM_VALUE : 1 - ZOOM_VALUE;
    setScale(scaleValue);
}

ViewportWidget::ViewportWidget(QWidget *parent) : _scene(this) {
    setScene(&_scene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::ScrollHandDrag);
    _scene.setBackgroundBrush(QBrush(Qt::gray));
}

void ViewportWidget::setImage(const Image& image) {
    _scene.clear();
    _items.clear();
    _items.emplace_back(image, _scene.addPixmap(QPixmap::fromImage(image.getImage())));
}
void ViewportWidget::addImage(const Image &image) {
    QGraphicsPixmapItem *itemPixmap = _scene.addPixmap(QPixmap::fromImage(image.getImage()));
    _items.emplace_back(image, itemPixmap);
}

void ViewportWidget::setScale(float scaleValue) {
    if (scaleValue < ZOOM_MIN || scaleValue > ZOOM_MAX) return;
    _currentScale *= scaleValue;
    scale(scaleValue, scaleValue);
    // QPointF mousePos = mapToScene(event->position().toPoint());
    // centerOn(mousePos);
}
} // namespace App