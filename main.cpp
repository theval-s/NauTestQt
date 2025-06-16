#include "main_window.hpp"
#include "viewport_widget.hpp"

#include <QApplication>
#include <QImage>
#include <QImageReader>
#include <QMainWindow>
#include <QPushButton>
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    App::MainWindow w(1920, 1080);
    w.show();

    return QApplication::exec();
}