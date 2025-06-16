//
// Created by Volkov Sergey on 16/06/2025.
//

#include "inspector_widget.hpp"

#include <QDoubleSpinBox>
#include <QLabel>
#include <QScrollArea>

namespace App {

void InspectorWidget::setupUi() {
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    QWidget *content_widget = new QWidget(this);
    _contentLayout = new QVBoxLayout(content_widget);
    _contentLayout->setSpacing(2);
    _contentLayout->setContentsMargins(11, 11, 11, 11);

    QScrollArea *scroll_area = new QScrollArea();
    scroll_area->setWidgetResizable(true);
    scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_area->setWidget(content_widget);

    main_layout->addWidget(scroll_area);
}
void InspectorWidget::createTransformGroup() {
    QWidget *transform_widget = new QWidget(this);
    QVBoxLayout *transform_layout = new QVBoxLayout(transform_widget);
    transform_layout->addWidget(new QLabel("Transform", transform_widget));
    _contentLayout->addWidget(transform_widget);

    //Position
    QHBoxLayout *position_layout = new QHBoxLayout();
    position_layout->setSpacing(2);
    transform_layout->addLayout(position_layout);
    position_layout->addWidget(new QLabel("Position", transform_widget));

    _posX = new QDoubleSpinBox(transform_widget);
    _posX->setSingleStep(1.f);
    _posX->setValue(0);
    connect(_posX, &QDoubleSpinBox::valueChanged, this,
            &InspectorWidget::onTransformPropertyChanged);
    _posY = new QDoubleSpinBox(transform_widget);
    connect(_posY, &QDoubleSpinBox::valueChanged, this,
            &InspectorWidget::onTransformPropertyChanged);
    _posY->setSingleStep(0.1f);
    _posY->setValue(0);

    position_layout->addWidget(_posX);
    position_layout->addWidget(_posY);

    //Scale
    QHBoxLayout *scale_layout = new QHBoxLayout();
    scale_layout->setSpacing(2);
    transform_layout->addLayout(scale_layout);
    scale_layout->addWidget(new QLabel("Scale", transform_widget));

    _scaleX = new QDoubleSpinBox(transform_widget);
    _scaleX->setSingleStep(0.2f);
    _scaleX->setValue(1.0f);
    _scaleX->setMinimum(0.0f);
    connect(_scaleX, &QDoubleSpinBox::valueChanged, this,
            &InspectorWidget::onTransformPropertyChanged);
    _scaleY = new QDoubleSpinBox(transform_widget);
    _scaleY->setSingleStep(0.5f);
    _scaleY->setValue(1.0f);
    _scaleY->setMinimum(0.0f);
    connect(_scaleY, &QDoubleSpinBox::valueChanged, this,
            &InspectorWidget::onTransformPropertyChanged);

    scale_layout->addWidget(_scaleX);
    scale_layout->addWidget(_scaleY);

    //Rotation
    QHBoxLayout *rotation_layout = new QHBoxLayout();
    rotation_layout->setSpacing(2);
    transform_layout->addLayout(rotation_layout);
    rotation_layout->addWidget(new QLabel("Rotation", transform_widget));

    _rotation = new QDoubleSpinBox(transform_widget);
    _rotation->setSingleStep(0.5f);
    _rotation->setValue(0.0f);
    _rotation->setMinimum(-360.f);
    _rotation->setMaximum(360.f);
    connect(_rotation, &QDoubleSpinBox::valueChanged, this,
            &InspectorWidget::onTransformPropertyChanged);
    rotation_layout->addWidget(_rotation);

    //Flips
    QHBoxLayout *flip_layout = new QHBoxLayout();
    flip_layout->setSpacing(10);
    transform_layout->addLayout(flip_layout);

    _flipX = new QCheckBox("Flip Horizontal");
    _flipX->setChecked(false);
    connect(_flipX, &QCheckBox::checkStateChanged, this,
            &InspectorWidget::onTransformPropertyChanged);
    _flipY = new QCheckBox("Flip Vertical");
    _flipY->setChecked(false);
    connect(_flipY, &QCheckBox::checkStateChanged, this,
            &InspectorWidget::onTransformPropertyChanged);

    flip_layout->addWidget(_flipX);
    flip_layout->addWidget(_flipY);
}
void InspectorWidget::createAppearanceGroup() {
    QWidget *appearance_widget = new QWidget(this);
    QVBoxLayout *appearance_layout = new QVBoxLayout(appearance_widget);
    appearance_layout->setSpacing(2);
    appearance_layout->addWidget(new QLabel("Appearance", appearance_widget));
    _contentLayout->addWidget(appearance_widget);

    //Visibility
    _itemVisible = new QCheckBox("Visible");
    _itemVisible->setChecked(true);
    appearance_layout->addWidget(_itemVisible);
    connect(_itemVisible, &QCheckBox::toggled, this,
            &InspectorWidget::itemVisibleModified);

    //Opacity
    QHBoxLayout *opacity_layout = new QHBoxLayout();
    opacity_layout->setSpacing(2);
    appearance_layout->addLayout(opacity_layout);
    opacity_layout->addWidget(new QLabel("Opacity", appearance_widget));

    _opacity = new QSlider(Qt::Horizontal, appearance_widget);
    _opacity->setValue(100.0f);
    _opacity->setRange(0, 100);
    connect(_opacity, &QSlider::valueChanged, this, [this]() {
        const qreal new_opacity = _opacity->value() / 100.0f;
        emit opacityModified(new_opacity);
    });
    opacity_layout->addWidget(_opacity);

    //Z-value (layer)
    QHBoxLayout *z_value_layout = new QHBoxLayout();
    z_value_layout->setSpacing(10);
    z_value_layout->addWidget(new QLabel("Z value (layer)", appearance_widget));
    appearance_layout->addLayout(z_value_layout);

    _zValue = new QDoubleSpinBox(appearance_widget);
    _zValue->setSingleStep(1.f);
    _zValue->setValue(0);
    _zValue->setMinimum(-100.f);
    _zValue->setMaximum(100.f);
    connect(_zValue, &QDoubleSpinBox::valueChanged, this,
            &InspectorWidget::zValueModified);
    appearance_layout->addWidget(_zValue);
}
void InspectorWidget::createDisplayGroup() {
    QWidget *display_widget = new QWidget(this);
    QVBoxLayout *display_layout = new QVBoxLayout(display_widget);
    display_layout->setSpacing(10);
    display_layout->addWidget(new QLabel("Editor Settings", display_widget));
    _contentLayout->addWidget(display_widget);

    //CheckBoxes
    _showPath = new QCheckBox("Show Path", display_widget);
    _showPath->setChecked(true);
    connect(_showPath, &QCheckBox::checkStateChanged, this,
            &InspectorWidget::onDisplayOptionsChanged);
    display_layout->addWidget(_showPath);
    _showResolution = new QCheckBox("Show Resolution", display_widget);
    _showResolution->setChecked(true);
    connect(_showResolution, &QCheckBox::checkStateChanged, this,
            &InspectorWidget::onDisplayOptionsChanged);
    display_layout->addWidget(_showResolution);

    //Zoom spinbox
    QHBoxLayout *zoom_layout = new QHBoxLayout();
    zoom_layout->setSpacing(2);
    display_layout->addLayout(zoom_layout);
    zoom_layout->addWidget(new QLabel("Zoom step", display_widget));

    _zoom = new QDoubleSpinBox(display_widget);
    _zoom->setSingleStep(0.05f);
    _zoom->setMinimum(0.001f);
    _zoom->setValue(0.1f);
    connect(_zoom, &QDoubleSpinBox::valueChanged, this,
            &InspectorWidget::onDisplayOptionsChanged);
    zoom_layout->addWidget(_zoom);
}
float InspectorWidget::roundToStep(float val, float step) {
    return std::round(val / step) * step;
}
void InspectorWidget::onTransformPropertyChanged() {
    QTransform transform;
    transform.translate(_posX->value(), _posY->value());
    transform.scale(_scaleX->value(), _scaleY->value());
    transform.rotate(_rotation->value());

    if (_flipX->isChecked()) transform.scale(-1.0f, 1.0f);
    if (_flipY->isChecked()) transform.scale(1.0f, -1.0f);
    emit transformModified(transform);
}
void InspectorWidget::onDisplayOptionsChanged() {
    DisplayOptions new_opts;
    new_opts.showPath = _showPath->isChecked();
    new_opts.showResolution = _showResolution->isChecked();
    new_opts.zoomValue = _zoom->value();
    emit displayOptionsModified(new_opts);
}
void InspectorWidget::loadImage(const Image &image) {

    //Parsing transform:
    const QTransform tf = image.transform;
    _posX->setValue(tf.dx()); //not sure if I have to round this
    _posY->setValue(tf.dy());
    _scaleX->setValue(roundToStep(std::hypot(tf.m11(), tf.m12()), 0.1f));
    _scaleY->setValue(roundToStep(std::hypot(tf.m21(), tf.m22()), 0.1f));

    double rot_radians = std::atan2(tf.m21(), tf.m11());
    _rotation->setValue(roundToStep(rot_radians * (180.0 / M_PI), 0.5f));
    _opacity->setValue(image.opacity*100.f);
    _zValue->setValue(image.zValue);
}

} // namespace App