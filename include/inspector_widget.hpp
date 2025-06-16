//
// Created by Volkov Sergey on 16/06/2025.
//

#pragma once

#include "editor_settings.hpp"
#include "image.hpp"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

namespace App {

/// @class InspectorWidget is a widget that allows modifying properties of an Image from GUI
class InspectorWidget : public QWidget {
    Q_OBJECT
    ///@brief Base layout of Inspector groups
    QVBoxLayout *_contentLayout = nullptr;

    //Transform widgets
    QDoubleSpinBox *_posX = nullptr, *_posY = nullptr;
    QDoubleSpinBox *_scaleX = nullptr, *_scaleY = nullptr;
    QDoubleSpinBox *_rotation = nullptr;
    QCheckBox *_flipX = nullptr, *_flipY = nullptr;

    //Appearance widgets
    QSlider *_opacity = nullptr;
    QCheckBox *_itemVisible = nullptr;
    QDoubleSpinBox *_zValue = nullptr;

    //Editor Options value widgets
    QCheckBox *_showPath = nullptr, *_showResolution = nullptr;
    QDoubleSpinBox *_zoom = nullptr;

    //Functions that set up the UI widgets
    void setupUi();
    /// @brief A group of widgets to represent Image transform.
    /// Has widgets for inputting position, rotation, scale and flips.
    /// Invokes onTransformPropertyChanged slot on any change
    void createTransformGroup();
    /// @brief A group of widgets for appearance of Image (layer, opacity)
    /// Has widgets for inputting opacity, layer or visibility.
    /// Emits opacityModified(), itemVisibleModified and zValueModified
    /// on respective changes
    void createAppearanceGroup();
    /// @brief A group of widgets for generic settings not related to Image project
    /// Has zoom value, showPath and showResolution.
    /// Emits editorSettingsModified on any change
    void createEditorGroup();

    /// @brief round @param val to closest @param step
    /// @return @param val rounded to @param step
    static float roundToStep(float val, float step);

  public:
    /// @brief Constructs and sets up InspectorWidget
    explicit InspectorWidget(QWidget *parent = nullptr);
  signals:
    void transformModified(const QTransform &transform);
    void opacityModified(qreal opacity);
    void itemVisibleModified(bool visible);
    void zValueModified(double zValue);
    void editorSettingsModified(const EditorSettings options);

  private slots:
    /// @brief Converts all single values of TransformGroup to QTransform.
    /// Emits transformModified
    void onTransformPropertyChanged();
    void onDisplayOptionsChanged();

  public slots:
    /// @brief Sets up all values in groups to those of an @param image.
    void loadImage(const Image &image);
};

} // namespace App
