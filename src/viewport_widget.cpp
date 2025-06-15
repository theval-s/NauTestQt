//
// Created by Volkov Sergey on 12/06/2025.
//

#include "viewport_widget.hpp"

namespace App {

void ViewportWidget::wheelEvent(QWheelEvent *event) {
    const float scaleValue =
        event->angleDelta().y() > 0 ? 1 + ZOOM_VALUE : 1 - ZOOM_VALUE;
    setScale(scaleValue);
    event->accept();
}

ViewportWidget::ViewportWidget(QWidget *parent) : _scene(this) {
    setScene(&_scene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    _scene.setBackgroundBrush(QBrush(Qt::gray));
}

void ViewportWidget::setImage(const Image &image) {
    _scene.clear();
    _items.clear();
    maxHeight = image.getImage().height(), maxWidth = image.getImage().width();
    _items.emplace_back(image,
                        _scene.addPixmap(QPixmap::fromImage(image.getImage())));
    updateSceneRect();
    centerOn(maxWidth / 2, maxHeight / 2);
}
void ViewportWidget::addImage(const Image &image) {
    QGraphicsPixmapItem *itemPixmap =
        _scene.addPixmap(QPixmap::fromImage(image.getImage()));
    maxHeight = qMax(itemPixmap->boundingRect().height(), maxHeight);
    maxWidth = qMax(itemPixmap->boundingRect().width(), maxWidth);


    _items.emplace_back(image, itemPixmap);

    updateSceneRect();
    centerOn(maxWidth / 2, maxHeight / 2);
}

void ViewportWidget::setScale(float scaleValue) {
    float newScale = _currentScale * scaleValue;
    if (newScale < ZOOM_MIN || newScale > ZOOM_MAX ) return;
    _currentScale = newScale;
    scale(scaleValue, scaleValue);
    updateSceneRect();
}
std::vector<Image> ViewportWidget::getImages() const {
    std::vector<Image> result;
    result.reserve(_items.size());
    for (const auto &p : _items) {
        result.push_back(p.first);
    }
    return result;
}
void ViewportWidget::updateSceneRect() {
    const QRectF viewRect = mapToScene(viewport()->rect()).boundingRect();
    QRectF pixmapRect(0,0,maxWidth,maxHeight);
    qreal marginX = viewRect.width() *0.5f;
    qreal marginY = viewRect.height() *0.5f;
    pixmapRect = pixmapRect.adjusted(-marginX, -marginY, marginX, marginY);
    _scene.setSceneRect(pixmapRect);
}
} // namespace App