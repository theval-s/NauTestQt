//
// Created by Volkov Sergey on 16/06/2025.
//

#pragma once

#include "image.hpp"
#include "display_options.hpp"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

namespace App {

class InspectorWidget: public QWidget {
Q_OBJECT
    QVBoxLayout* _contentLayout = nullptr;


    //Transform value widgets
    QDoubleSpinBox* _posX = nullptr, *_posY = nullptr;
    QDoubleSpinBox* _scaleX = nullptr, *_scaleY = nullptr;
    QDoubleSpinBox* _rotation = nullptr;
    QCheckBox* _flipX = nullptr, *_flipY = nullptr;

    //Appearance value widgets
    QSlider* _opacity = nullptr;
    QCheckBox* _itemVisible = nullptr;

    //Display Options value widgets
    QCheckBox *_showPath = nullptr, *_showResolution = nullptr;
    QDoubleSpinBox *_zoom = nullptr;

    void setupUi();
    void createTransformGroup();
    void createAppearanceGroup();
    void createDisplayGroup();

    float roundToStep(float val, float step);

public:
    explicit InspectorWidget(QWidget *parent = nullptr) {
        setParent(parent);
        setupUi();
        createTransformGroup();
        createAppearanceGroup();
        createDisplayGroup();
    }
    signals:
    void transformModified(const QTransform &transform);
    void opacityModified(qreal opacity);
    void itemVisibleModified(bool visible);
    void displayOptionsModified(const DisplayOptions options);

    private slots:
    void onTransformPropertyChanged();
    void onDisplayOptionsChanged();

    public slots:
    void loadImage(const Image &image);

};

} // App
