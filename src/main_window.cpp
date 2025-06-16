//
// Created by Volkov Sergey on 16/06/2025.
//

#include "saving_manager.hpp"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>
#include <main_window.hpp>

namespace App {

void MainWindow::setupStatusBar() {
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
void MainWindow::menuBarSetup() {
    //menu initialisation
    QMenuBar *menu = new QMenuBar(this);
    QMenu *file_menu = menu->addMenu("File");
    if (_openAction) file_menu->QWidget::addAction(_openAction);
    if (_saveAction) file_menu->QWidget::addAction(_saveAction);
    setMenuBar(menu);
}
void MainWindow::setupToolBar() {
    QToolBar *toolbar = addToolBar("Main Toolbar");
    if (_openAction) toolbar->QWidget::addAction(_openAction);
    if (_openAction) toolbar->QWidget::addAction(_saveAction);
    QAction *zoomInAction = new QAction("Zoom In", toolbar);
    QAction *zoomOutAction = new QAction("Zoom Out", toolbar);
    QAction *resetAction = new QAction("Reset", toolbar);
    zoomInAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ZoomIn));
    zoomInAction->setToolTip("Zoom In");
    zoomOutAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ZoomOut));
    zoomOutAction->setToolTip("Zoom Out");
    resetAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::EditClear));
    resetAction->setToolTip("Reset image transform");
    if (_viewport) {
        connect(zoomInAction, &QAction::triggered, _viewport,
                &ViewportWidget::zoomIn);
        connect(zoomOutAction, &QAction::triggered, _viewport,
                &ViewportWidget::zoomOut);
        connect(resetAction, &QAction::triggered, this, &MainWindow::resetTransform);
    }

    toolbar->addAction(zoomInAction);
    toolbar->addAction(zoomOutAction);
    toolbar->addAction(resetAction);
    toolbar->addSeparator();
}
void MainWindow::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *main_layout = new QHBoxLayout(centralWidget);
    QVBoxLayout *side_layout = new QVBoxLayout();
    main_layout->addLayout(side_layout);

    if (_hierarchy) {
        side_layout->addWidget(_hierarchy, 1);
    }

    if (_inspector) {
        side_layout->addWidget(_inspector, 2);
    }
    if (_viewport) main_layout->addWidget(_viewport, 2);

    _openAction = new QAction("Open", this);
    _saveAction = new QAction("Save", this);
    _openAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew));
    _saveAction->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DocumentSave));

    menuBarSetup();
    setupStatusBar();
    setupToolBar();
}
void MainWindow::openFile() {
    QString file = QFileDialog::getOpenFileName(
        this, "Open...", QDir::homePath(),
        "Supported files (*.png *.jpg *jpeg *webm *.bmp *.json)");
    if (!file.isEmpty() && _viewport) {
        try {
            if (file.endsWith(".json")) {
                std::vector<Image> images = SavingManager::loadProject(file);
                if (images.size() > 0) _viewport->setImage(images[0]);
                if (images.size() > 1)
                    for (size_t i = 1; i < images.size(); i++)
                        _viewport->addImage(images[i]);
            } else {
                Image img(file);
                _viewport->addImage(img);
            }
        } catch (const std::exception &e) {
            QMessageBox::critical(this, "Error!", e.what());
        }
    }
}
void MainWindow::saveFile() {
    if (_viewport) try {
            QString file = QFileDialog::getSaveFileName(
                this, "Save to...", QDir::homePath(), "JSON File (*.json)");
            qDebug() << "Saving to..." << file;
            if (!file.endsWith(".json")) file += ".json";
            SavingManager::saveProject(_viewport->getImages(), file);
        } catch (const std::exception &e) {
            QMessageBox::critical(this, "Error!", e.what());
        }
}
void MainWindow::connectSignals() {
    connect(this, &MainWindow::activeImageChanged, this,
            &MainWindow::updatePathLabel);
    if (_viewport) {
        //to viewport
        connect(this, &MainWindow::zoomValueChanged, _viewport,
                &ViewportWidget::setMouseZoom);
        connect(_openAction, &QAction::triggered, this, &MainWindow::openFile);
        connect(_saveAction, &QAction::triggered, this, &MainWindow::saveFile);

        //from viewport
        connect(_viewport, &ViewportWidget::imagesChanged, this,
                &MainWindow::handleImageUpdate);
        connect(_viewport, &ViewportWidget::scaleChanged, this,
                &MainWindow::updateScaleLabel);
        connect(_viewport, &ViewportWidget::imagesChanged, this,
                &MainWindow::updateResolutionLabel);
        if (_hierarchy)
            connect(_viewport, &ViewportWidget::imagesChanged, _hierarchy,
                    &HierarchyWidget::updateList);
    }
    if (_inspector) {
        //to inspector
        connect(this, &MainWindow::activeImageChanged, _inspector,
                &InspectorWidget::loadImage);

        //from inspector
        connect(_inspector, &InspectorWidget::displayOptionsModified, this,
                &MainWindow::updateEditorSettings);
        if (_viewport) {
            connect(_inspector, &InspectorWidget::transformModified, _viewport,
                    [this](const QTransform &transform) {
                        _viewport->setTransform(_imageIndex, transform);
                    });
            connect(_inspector, &InspectorWidget::itemVisibleModified, this,
                    [this](bool visible) {
                        _viewport->setItemVisible(_imageIndex, visible);
                    });
            connect(_inspector, &InspectorWidget::opacityModified, this,
                    [this](float opacity) {
                        _viewport->setOpacity(_imageIndex, opacity);
                    });
            connect(_inspector, &InspectorWidget::zValueModified, this,
                    [this](float zValue) {
                        _viewport->setItemZValue(_imageIndex, zValue);
                    });
        }
    }
    if (_hierarchy) {
        connect(_hierarchy, &QListWidget::currentRowChanged, this,
                [this](int row) {
                    this->_imageIndex = row;
                    if (_viewport)
                        emit activeImageChanged(
                            _viewport->getImages().at(_imageIndex));
                });
    }
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _viewport(new ViewportWidget(this)),
      _inspector(new InspectorWidget), _hierarchy(new HierarchyWidget) {
    setupUi();
    connectSignals();
}
MainWindow::MainWindow(int width, int height, QWidget *parent)
    : QMainWindow(parent), _viewport(new ViewportWidget(this)),
      _inspector(new InspectorWidget), _hierarchy(new HierarchyWidget) {
    this->resize(width, height);
    setupUi();
    connectSignals();
}
void MainWindow::updatePathLabel(const Image &image) {
    _pathLabel->setText(image.getFileInfo().absoluteFilePath());
}
void MainWindow::updateScaleLabel(float scale) {
    _scaleLabel->setText(QString::number(scale * 100, 'f', 0) + "%");
}
void MainWindow::updateResolutionLabel() {
    QRect rect = _viewport->getImageRectSize();
    QString res =
        QString::number(rect.width()) + "x" + QString::number(rect.height());
    _resolutionLabel->setText(res);
}
void MainWindow::updateEditorSettings(const EditorSettings &options) {
    if (options.showPath != _displayOptions.showPath) {
        if (options.showPath) {
            _pathLabel->setVisible(true);
            if (_viewport)
                connect(this, &MainWindow::activeImageChanged, this,
                        &MainWindow::updatePathLabel);
        } else {
            _pathLabel->setVisible(false);
            if (_viewport)
                disconnect(this, &MainWindow::activeImageChanged, this,
                           &MainWindow::updatePathLabel);
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
        _imageIndex = 0;
    } else if (images.size() == 1) {
        _imageIndex = 0;
        emit activeImageChanged(images[_imageIndex]);
    }
}
void MainWindow::resetTransform() {
    if (_viewport) {
        _viewport->setTransform(_imageIndex, QTransform());
        emit activeImageChanged(_viewport->getImages().at(_imageIndex));
    }
}
} // namespace App