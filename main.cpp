#include "main_window.hpp"
#include "viewport_widget.hpp"

#include <QApplication>
#include <QImage>
#include <QImageReader>
#include <QMainWindow>
#include <QPushButton>
#include <iostream>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    App::MainWindow w(800, 600);
    w.show();

    // App::Image img("/Users/val/Pictures/finalspace2.png");
    // qDebug() << img.getFileInfo() << (img.getFileInfo().exists()?"exists":"does not exist");
    // img.opacity = 0.5f;
    // img.transform.rotate(0);
    // App::ViewportWidget viewport;
    // viewport.addImage(img);
    // viewport.show();

    return QApplication::exec();
}