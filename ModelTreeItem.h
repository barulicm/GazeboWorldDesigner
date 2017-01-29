#ifndef GWD_MODELTREEITEM_H
#define GWD_MODELTREEITEM_H

#include <QString>
#include <QList>

class ModelTreeItem {
public:
    explicit ModelTreeItem(const QString &name, const QString &gazeboName, ModelTreeItem *parentItem);
    ~ModelTreeItem();

    void appendChild(ModelTreeItem *item);

    ModelTreeItem *child(int row);

    int childCount() const;

    int columnCount() const;

    QString name() const;

    QString gazeboName() const;

    int row() const;

    ModelTreeItem *parentItem();

private:
    QList<ModelTreeItem*> childItems;
    QString humanReadableName;
    QString gazeboModelName;
    ModelTreeItem *parent;
};


#endif //GWD_MODELTREEITEM_H
