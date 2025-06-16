//
// Created by Volkov Sergey on 12/06/2025.
//

#pragma once
#include "image.hpp"

#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QWheelEvent>

namespace App {
constexpr float ZOOM_MIN = 0.1f; //possible to extract those to settings too
constexpr float ZOOM_MAX = 10.0f; //but I don't see the reason right now

/// @class ViewportWidget handles displaying Images, including pannable and zoomable area
class ViewportWidget : public QGraphicsView {
    Q_OBJECT

    //Needed variables
    float _currentScale = 1.0f;
    //keeping a pointer to scene might be useful for handling separate scenes
    //but as of right now there is only one scene, although several images are possible.
    //modifying to pointer would be easy.
    QGraphicsScene _scene;
    /// Vector of Images and their PixmapItems. Items are owned by _scene
    std::vector<std::pair<Image, QGraphicsPixmapItem *>> _items;
    float _mouseZoomValue = 0.1f;

    void wheelEvent(QWheelEvent *event) override;

  public:

    //
    explicit ViewportWidget(QWidget *parent = nullptr);
    // ~ViewportWidget();
    /// @brief Clears the scene and sets @param image as the new only Image on scene
    void setImage(const Image &image);
    /// @brief Adds image to existing scene (even if scene is empty)
    void addImage(const Image &image);
    /// @brief Size of the rectangle bounding the _items. Used as resolution.
    QRect getImageRectSize() const {
        return _scene.itemsBoundingRect().toRect();
    }
    /// @brief Removes item from scene and _items
    void removeImage(const size_t imageIndex);
    float getScale() const { return _currentScale; }
    [[nodiscard]] std::vector<Image> getImages() const;
    /// @brief Updates sceneRect to allow panning properly
    void updateSceneRect();
    /// @brief Sets all the variables from img to _items[pixmap_ind]
    void applyParametersToPixmapItem(const Image &img, size_t pixmap_ind);

  signals:
    ///@brief Emitted on any modification to
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
