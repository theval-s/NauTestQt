//
// Created by Volkov Sergey on 16/06/2025.
//

#include <main_window.hpp>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>

namespace App {

void MainWindow::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *main_layout = new QHBoxLayout(centralWidget);

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

    if (_viewport) {
        connect(_viewport, &ViewportWidget::scaleChanged, this, &MainWindow::updateScaleLabel);
        connect(_viewport, &ViewportWidget::imagesChanged, this, &MainWindow::updatePathLabel);
        connect(_viewport, &ViewportWidget::imagesChanged, this, &MainWindow::updateResolutionLabel);

        connect(_openAction, &QAction::triggered, [this]() {
            QString file = QFileDialog::getOpenFileName(this, "Open...", QDir::homePath(), "Images (*.png *.jpg *jpeg *webm)");
            if (!file.isEmpty()) {
                Image img(file);
                _viewport->addImage(img);
            }
        });
    }

}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _viewport(new ViewportWidget(this)) {
    setupUi();
    connectSignals();
}
MainWindow::MainWindow(int width, int height, QWidget *parent)
    : QMainWindow(parent), _viewport(new ViewportWidget(this)) {
    this->resize(width, height);
    setupUi();
    connectSignals();
}
void MainWindow::updatePathLabel() {
    std::vector<Image> images = _viewport->getImages();
    if (images.empty()) return;
    _pathLabel->setText(images[0].getFileInfo().absoluteFilePath());
}
void MainWindow::updateScaleLabel(float scale) {
    _scaleLabel->setText(QString::number(scale*100, 'f',0) + "%");
}
void MainWindow::updateResolutionLabel() {
    QRect rect = _viewport->getImageRectSize();
    QString res = QString::number(rect.width())+ "x" + QString::number(rect.height());
    _resolutionLabel->setText(res);
}
} // App