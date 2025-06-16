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
constexpr float ZOOM_MAX = 10.0f;

class ViewportWidget : public QGraphicsView {
    Q_OBJECT

    float _currentScale = 1.0f;
    QGraphicsScene _scene;
    std::vector<std::pair<Image, QGraphicsPixmapItem *>> _items;
    float _mouseZoomValue = 0.1f;

    void wheelEvent(QWheelEvent *event) override;

  public:
    explicit ViewportWidget(QWidget *parent = nullptr);
    // ~ViewportWidget();
    void setImage(const Image &image);
    void addImage(const Image &image);
    QRect getImageRectSize() const {
        return _scene.itemsBoundingRect().toRect();
    }
    void removeImage(const size_t imageIndex);
    float getScale() const { return _currentScale; }
    [[nodiscard]] std::vector<Image> getImages() const;
    void updateSceneRect();
    void applyParametersToPixmapItem(const Image &img, size_t pixmap_ind);

  signals:
    void imagesChanged(const std::vector<Image> &images);
    void scaleChanged(float newScale);
  public slots:
    void setScale(float scale);
    void zoomIn();
    void zoomOut();
    void setTransform(size_t index = 0,
                      QTransform pixmapTransform = QTransform());
    void setOpacity(size_t index = 0, float opacity = 1.0f);
    void setItemVisible(size_t index = 0, bool visible = 1);
    void setItemZValue(size_t index = 0, float zValue = 0);
    void setMouseZoom(float zoom);
};
} // namespace App
