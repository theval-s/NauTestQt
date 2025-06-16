//
// Created by Volkov Sergey on 16/06/2025.
//

#include "hierarchy_widget.hpp"

namespace App {

void HierarchyWidget::updateList(const std::vector<Image> &images) {
    this->blockSignals(true);
    this->clearSelection();
    this->clearFocus();
    this->clear();
    for (const auto &image : images) {
        this->addItem(image.getName());
    }
    this->blockSignals(false);
}
} // App