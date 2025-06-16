//
// Created by Volkov Sergey on 16/06/2025.
//

#pragma once

#include "image.hpp"

#include <QListWidget>
namespace App {

/// Widget that displays ordered list of all Images on current Viewport vector
class HierarchyWidget : public QListWidget {
    Q_OBJECT
  public:
    HierarchyWidget(QWidget *parent = nullptr) {
        setParent(parent);
        this->setSelectionBehavior(SelectItems);
        this->setSelectionMode(QAbstractItemView::SingleSelection);
        this->setDragEnabled(false);
    }

  public slots:
    ///Clears existing list and builds a new one
    ///@param images vector of Images to display in hierarchy
    void updateList(const std::vector<Image> &images);
};

} // namespace App
