//
// Created by Volkov Sergey on 12/06/2025.
//

#include "viewport_widget.hpp"

namespace App {

void ViewportWidget::wheelEvent(QWheelEvent *event) {
    //only using to zoom in/out so I think there's no need to bother
    //with angleDelta().x()
    const float scaleValue =
        event->angleDelta().y() > 0 ? 1 + _mouseZoomValue : 1 - _mouseZoomValue;
    setScale(scaleValue);
    event->accept();
}

ViewportWidget::ViewportWidget(QWidget *parent) : _scene(this) {
    //sets scene and needed settings
    setScene(&_scene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode(FullViewportUpdate);
    _scene.setBackgroundBrush(QBrush(Qt::gray));
}

void ViewportWidget::setImage(const Image &image) {
    _scene.clear();
    _items.clear();
    addImage(image);
}
void ViewportWidget::addImage(const Image &image) {
    QGraphicsPixmapItem *itemPixmap =
        _scene.addPixmap(QPixmap::fromImage(image.getImage()));
    //place for any options to itemPixmap
    _items.emplace_back(image, itemPixmap);
    applyParametersToPixmapItem(image, _items.size() - 1);
    updateSceneRect();
    centerOn(_scene.itemsBoundingRect().center());
    emit imagesChanged(this->getImages());
}
void ViewportWidget::removeImage(const size_t imageIndex) {
    if (imageIndex >= _items.size()) {
        throw std::out_of_range("Image index out of range");
    }
    _scene.removeItem(_items[imageIndex].second);
    _items.erase(_items.begin() + imageIndex);
    updateSceneRect();

    emit imagesChanged(this->getImages());
}

void ViewportWidget::setScale(float scaleValue) {
    float newScale = _currentScale * scaleValue;
    if (newScale < ZOOM_MIN) return;
    if (newScale > ZOOM_MAX) {
        //clamping to ZOOM_MAX
        if (std::abs(_currentScale-ZOOM_MAX)<1e-9) return;
        else newScale = ZOOM_MAX;
    }
    _currentScale = newScale;
    scale(scaleValue, scaleValue);
    updateSceneRect();
    emit scaleChanged(_currentScale);
}
void ViewportWidget::zoomIn() {
    setScale(1+_mouseZoomValue);
}
void ViewportWidget::zoomOut() {
    setScale(1-_mouseZoomValue);
}
void ViewportWidget::setTransform(size_t index, QTransform pixmapTransform) {
    if (index >= _items.size()) return;
    _items[index].second->setTransform(pixmapTransform);
    _items[index].first.transform = pixmapTransform;
    updateSceneRect();
}

//Rect is not changing in those, so no need to update it
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
    //there might be some better way with std::ranges
    //but I don't know them at all
    std::vector<Image> result;
    result.reserve(_items.size());
    for (const auto &p : _items) {
        result.push_back(p.first);
    }
    return result;
}
void ViewportWidget::updateSceneRect() {
    const QRectF viewRect = mapToScene(viewport()->rect()).boundingRect();
    //getting the bounds of the whole scene,
    //and applying margins to allow panning a bit past the screen
    QRectF pixmapRect = _scene.itemsBoundingRect();
    qreal marginX = viewRect.width() * 0.5f;
    qreal marginY = viewRect.height() * 0.5f;
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

    updateSceneRect();
}
} // namespace App