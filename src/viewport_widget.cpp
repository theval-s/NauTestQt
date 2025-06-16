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
void ViewportWidget::updateBounds() {
    qreal width = 0, height = 0;
    for (const auto p : _items) {
        width = qMax(p.second->sceneBoundingRect().width(), width);
        height = qMax(p.second->sceneBoundingRect().height(), height);
    }
    _maxWidth = width;
    _maxHeight = height;
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
    _maxWidth = 0, _maxHeight = 0;
    addImage(image);
}
void ViewportWidget::addImage(const Image &image) {
    QGraphicsPixmapItem *itemPixmap =
        _scene.addPixmap(QPixmap::fromImage(image.getImage()));
    itemPixmap->setTransformOriginPoint(itemPixmap->boundingRect().center());
    //place for any other options to itemPixmap

    _items.emplace_back(image, itemPixmap);
    applyParametersToPixmapItem(image, _items.size() - 1);
    updateSceneRect();
    centerOn(_maxWidth / 2, _maxHeight / 2);
}
void ViewportWidget::removeImage(const size_t imageIndex) {
    if (imageIndex >= _items.size()) {
        throw std::out_of_range("Image index out of range");
    }
    _items.erase(_items.begin() + imageIndex);
    updateBounds();

    emit imagesChanged(this->getImages());
}

void ViewportWidget::setScale(float scaleValue) {
    float newScale = _currentScale * scaleValue;
    if (newScale < ZOOM_MIN || newScale > ZOOM_MAX) return;
    _currentScale = newScale;
    scale(scaleValue, scaleValue);
    updateSceneRect();
    emit scaleChanged(_currentScale);
}
void ViewportWidget::zoomIn(float scale) {
    setScale(scale);
}
void ViewportWidget::zoomOut(float scale) {
    setScale(scale);
}
void ViewportWidget::setTransform(size_t index, QTransform pixmapTransform) {
    if (index >= _items.size()) return;
    _items[index].second->setTransform(pixmapTransform);
    _items[index].first.transform = pixmapTransform;
}
void ViewportWidget::setOpacity(size_t index, float opacity) {
    if (index >= _items.size()) return;
    _items[index].second->setOpacity(opacity);
    _items[index].first.opacity = opacity;
}
void ViewportWidget::setItemVisible(size_t index, bool visible) {
    if (index >= _items.size()) return;
    _items[index].second->setVisible(visible);
    _items[index].first.isVisible = visible;
}
void ViewportWidget::setItemZValue(size_t index, float zValue) {
    if (index >= _items.size()) return;
    _items[index].second->setZValue(zValue);
    _items[index].first.zValue = zValue;
}
void ViewportWidget::setMouseZoom(const float zoom) {
    _mouseZoomValue = zoom;
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
    QRectF pixmapRect(0,0,_maxWidth,_maxHeight);
    qreal marginX = viewRect.width() *0.5f;
    qreal marginY = viewRect.height() *0.5f;
    pixmapRect = pixmapRect.adjusted(-marginX, -marginY, marginX, marginY);
    _scene.setSceneRect(pixmapRect);
}

void ViewportWidget::applyParametersToPixmapItem(const Image &img,
                                                 const size_t pixmap_ind) {
    _items[pixmap_ind].second->setTransform(img.transform);
    _items[pixmap_ind].second->setOpacity(img.opacity);
    _items[pixmap_ind].second->setVisible(img.isVisible);
    _items[pixmap_ind].second->setZValue(img.zValue);

    //Border is not implemented as a part of this, as the better way
    //to do it is overriding paint of QGraphicsPixmapItem

    //Updating the actual rectangle size,
    //because rotation and scale might have changed it
    const QRectF rect = _items[pixmap_ind].second->sceneBoundingRect();
    _maxHeight = qMax(_maxHeight, rect.height());
    _maxWidth = qMax(_maxWidth, rect.width());

    emit imagesChanged(this->getImages());
    // qDebug() << maxHeight << " " << maxWidth;
}
} // namespace App