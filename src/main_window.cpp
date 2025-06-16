//
// Created by Volkov Sergey on 16/06/2025.
//

#include <main_window.hpp>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>

namespace App {

void MainWindow::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *main_layout = new QHBoxLayout(centralWidget);
    QVBoxLayout *side_layout = new QVBoxLayout();
    main_layout->addLayout(side_layout);

    if (_hierarchy) {
        side_layout->addWidget(_hierarchy,1);
    }

    if (_inspector) {
        side_layout->addWidget(_inspector, 2);
    }
    if (_viewport) main_layout->addWidget(_viewport, 2);

    //menu initialisation
    QMenuBar *menu = new QMenuBar(this);
    QMenu *file_menu = menu->addMenu("File");
    _openAction = new QAction("Open", this);
    file_menu->addAction(_openAction);
    setMenuBar(menu);

    //status_bar initialisation
    QStatusBar *status_bar = new QStatusBar(this);
    _pathLabel = new QLabel(this);
    _pathLabel->setText("Path");
    _resolutionLabel = new QLabel(this);
    _resolutionLabel->setText("Resolution");
    _scaleLabel = new QLabel(this);
    _scaleLabel->setText("100%");
    status_bar->addWidget(_pathLabel);
    status_bar->addWidget(_resolutionLabel);
    status_bar->addWidget(_scaleLabel);
    setStatusBar(status_bar);
}
void MainWindow::connectSignals() {
        connect(this, &MainWindow::activeImageChanged, this, &MainWindow::updatePathLabel);
    if (_viewport) {
        //to viewport
        connect(this, &MainWindow::zoomValueChanged, _viewport, &ViewportWidget::setMouseZoom);
        connect(_openAction, &QAction::triggered, [this]() {
                    QString file = QFileDialog::getOpenFileName(this, "Open...", QDir::homePath(), "Images (*.png *.jpg *jpeg *webm)");
                    if (!file.isEmpty()) {
                        Image img(file);
                        _viewport->addImage(img);
                    }
                });

        //from viewport
        connect(_viewport, &ViewportWidget::imagesChanged, this, &MainWindow::handleImageUpdate);
        connect(_viewport, &ViewportWidget::scaleChanged, this, &MainWindow::updateScaleLabel);
        connect(_viewport, &ViewportWidget::imagesChanged, this, &MainWindow::updateResolutionLabel);
        if (_hierarchy) connect(_viewport, &ViewportWidget::imagesChanged, _hierarchy, &HierarchyWidget::updateList);
    }
    if (_inspector) {
        //to inspector
        connect(this, &MainWindow::activeImageChanged, _inspector, &InspectorWidget::loadImage);

        //from inspector
        connect(_inspector, &InspectorWidget::displayOptionsModified, this, &MainWindow::updateDisplayOptions);
        if (_viewport) {
            connect(_inspector, &InspectorWidget::transformModified, _viewport, [this](const QTransform &transform) {
            _viewport->setTransform(imageIndex, transform);
        });
            connect(_inspector, &InspectorWidget::itemVisibleModified, this, [this](bool visible) {
            _viewport->setItemVisible(imageIndex, visible);
        });
            connect(_inspector, &InspectorWidget::opacityModified, this, [this](float opacity) {
                _viewport->setOpacity(imageIndex, opacity);
            });
            connect(_inspector, &InspectorWidget::zValueModified, this, [this](float zValue) {
                _viewport->setItemZValue(imageIndex, zValue);
            });
        }

    }
    if (_hierarchy) {
        connect(_hierarchy, &QListWidget::currentRowChanged, this, [this](int row) {
            this->imageIndex = row;
            if (_viewport) emit activeImageChanged(_viewport->getImages().at(imageIndex));
        });
    }
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _viewport(new ViewportWidget(this)),
 _inspector(new InspectorWidget), _hierarchy(new HierarchyWidget){
    setupUi();
    connectSignals();
}
MainWindow::MainWindow(int width, int height, QWidget *parent)
    : QMainWindow(parent), _viewport(new ViewportWidget(this)),
_inspector(new InspectorWidget), _hierarchy(new HierarchyWidget){
    this->resize(width, height);
    setupUi();
    connectSignals();
}
void MainWindow::updatePathLabel(const Image& image) {
    _pathLabel->setText(image.getFileInfo().absoluteFilePath());
}
void MainWindow::updateScaleLabel(float scale) {
    _scaleLabel->setText(QString::number(scale*100, 'f',0) + "%");
}
void MainWindow::updateResolutionLabel() {
    QRect rect = _viewport->getImageRectSize();
    QString res =
        QString::number(rect.width()) + "x" + QString::number(rect.height());
    _resolutionLabel->setText(res);
}
void MainWindow::updateDisplayOptions(const DisplayOptions &options) {
    if (options.showPath != _displayOptions.showPath) {
        if (options.showPath) {
            _pathLabel->setVisible(true);
            if (_viewport)
                connect(this, &MainWindow::activeImageChanged, this, &MainWindow::updatePathLabel);
        } else {
            _pathLabel->setVisible(false);
            if (_viewport)
                disconnect(this, &MainWindow::activeImageChanged, this, &MainWindow::updatePathLabel);
        }
    }
    if (options.showResolution != _displayOptions.showResolution) {
        if (options.showResolution) {
            _resolutionLabel->setVisible(true);
            if (_viewport)
                connect(_viewport, &ViewportWidget::imagesChanged, this,
                        &MainWindow::updateResolutionLabel);
        } else {
            _resolutionLabel->setVisible(false);
            if (_viewport)
                disconnect(_viewport, &ViewportWidget::imagesChanged, this,
                           &MainWindow::updateResolutionLabel);
        }
    }
    if (options.zoomValue != _displayOptions.zoomValue) {
        emit zoomValueChanged(options.zoomValue);
    }
    _displayOptions = options;
}
void MainWindow::handleImageUpdate(const std::vector<Image> &images) {
    if (images.empty()) {
        _pathLabel->setText("Path");
        _resolutionLabel->setText("Resolution");
        _scaleLabel->setText("Scale");
        imageIndex = 0;
    }
    // else {
    //     imageIndex = images.size() - 1;
    //     emit activeImageChanged(images[imageIndex]);
    // }
}
} // App