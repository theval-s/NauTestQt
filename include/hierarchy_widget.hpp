//
// Created by Volkov Sergey on 16/06/2025.
//

#pragma once

#include "image.hpp"

#include <QListWidget>
namespace App {

class HierarchyWidget: public QListWidget {
    Q_OBJECT
public:
    HierarchyWidget(QWidget* parent = nullptr) {
        setParent(parent);
        this->setSelectionBehavior(SelectItems);
        this->setSelectionMode(QAbstractItemView::SingleSelection);
        this->setDragEnabled(false);
    }

    public slots:
    void updateList(const std::vector<Image> &images);
    signals:


};

} // App
