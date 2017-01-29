#include "ModelTreeItem.h"

ModelTreeItem::ModelTreeItem(const QString &name, const QString &gazeboName, ModelTreeItem *parentItem) {
    parent = parentItem;
    humanReadableName = name;
    gazeboModelName = gazeboName;
}

ModelTreeItem::~ModelTreeItem() {
    qDeleteAll(childItems);
}

void ModelTreeItem::appendChild(ModelTreeItem *item) {
    childItems.append(item);
}

ModelTreeItem *ModelTreeItem::child(int row) {
    return childItems.value(row);
}

int ModelTreeItem::childCount() const {
    return childItems.size();
}

int ModelTreeItem::columnCount() const {
    return 1;
}

QString ModelTreeItem::name() const {
    return humanReadableName;
}

QString ModelTreeItem::gazeboName() const {
    return gazeboModelName;
}

int ModelTreeItem::row() const {
    if (parent != nullptr)
        return parent->childItems.indexOf(const_cast<ModelTreeItem*>(this));

    return 0;
}

ModelTreeItem *ModelTreeItem::parentItem() {
    return parent;
}
