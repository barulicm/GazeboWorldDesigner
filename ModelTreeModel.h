#ifndef GWD_MODELTREEMODEL_H
#define GWD_MODELTREEMODEL_H

#include <QAbstractItemModel>
#include "ModelTreeItem.h"

class ModelTreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit ModelTreeModel(QObject *parent = 0);

    ~ModelTreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::DropActions supportedDropActions() const override;

    QStringList mimeTypes() const override;

    QMimeData *mimeData(const QModelIndexList &indexes) const override;

private:

    ModelTreeItem *rootItem;

    void searchForROSModels();

    void searchForDownloadedModels();

    QVector<QPair<QString, QString>> findModelsInPath(QString directory);

};


#endif //GWD_MODELTREEMODEL_H
