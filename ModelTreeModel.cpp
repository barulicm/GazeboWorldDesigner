#include "ModelTreeModel.h"
#include <ros/package.h>
#include <QDirIterator>
#include <QDomDocument>
#include <iostream>
#include <QStandardPaths>
#include <QDataStream>
#include <QMimeData>
#include <QVector>
#include <QPair>

ModelTreeModel::ModelTreeModel(QObject *parent) : QAbstractItemModel(parent) {
    rootItem = new ModelTreeItem{"Models","", nullptr};

    searchForDownloadedModels();
    searchForROSModels();
}

ModelTreeModel::~ModelTreeModel() {
    delete rootItem;
}

QVariant ModelTreeModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid())
        return QVariant();

    if(role != Qt::DisplayRole)
        return QVariant();

    ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());

    return item->name();
}

Qt::ItemFlags ModelTreeModel::flags(const QModelIndex &index) const {
    Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

    ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());

    if(index.isValid() && item->childCount() == 0)
        return Qt::ItemIsDragEnabled | defaultFlags;
    else
        return defaultFlags;
}

QModelIndex ModelTreeModel::index(int row, int column, const QModelIndex &parent) const {

    if(!hasIndex(row, column, parent))
        return QModelIndex();

    ModelTreeItem *parentItem;

    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelTreeItem*>(parent.internalPointer());

    ModelTreeItem *childItem = parentItem->child(row);

    if(childItem != nullptr)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex ModelTreeModel::parent(const QModelIndex &index) const {
    if(!index.isValid())
        return QModelIndex();

    ModelTreeItem *childItem = static_cast<ModelTreeItem*>(index.internalPointer());
    ModelTreeItem *parentItem = childItem->parentItem();

    if(parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ModelTreeModel::rowCount(const QModelIndex &parent) const {
    ModelTreeItem *parentItem;
    if(parent.column() > 0)
        return 0;

    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int ModelTreeModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return static_cast<ModelTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

Qt::DropActions ModelTreeModel::supportedDropActions() const {
    return Qt::CopyAction;
}

QStringList ModelTreeModel::mimeTypes() const {
    return QStringList{"text/plain"};
}

QMimeData *ModelTreeModel::mimeData(const QModelIndexList &indexes) const {
    QMimeData *mimeData = new QMimeData;
    QString encodedData;
    for(const QModelIndex &index : indexes) {
        if(index.isValid()) {
            ModelTreeItem *item = static_cast<ModelTreeItem*>(index.internalPointer());
            QString gazeboName = item->gazeboName();
            QString humanName = item->name();
            encodedData += gazeboName;
        }
    }
    mimeData->setText(encodedData);
    return mimeData;
}

void ModelTreeModel::searchForROSModels() {
    std::vector<std::pair<std::string,std::string>> model_paths;
    ros::package::getPlugins("gazebo_ros", "gazebo_model_path", model_paths, true);
    for(const auto& pair : model_paths) {
        auto packageItem = new ModelTreeItem{pair.first.c_str(), "", rootItem};
        rootItem->appendChild(packageItem);

        auto models = findModelsInPath(pair.second.c_str());
        for(const auto &model : models) {
            auto modelItem = new ModelTreeItem{model.first, model.second, packageItem};
            packageItem->appendChild(modelItem);
        }
    }
}

void ModelTreeModel::searchForDownloadedModels() {
    QDir dir{QStandardPaths::writableLocation(QStandardPaths::HomeLocation)};
    dir = QDir{dir.filePath(".gazebo")};
    if(!dir.exists())
        return;

    auto packageItem = new ModelTreeItem{"Downloaded Models", "", rootItem};
    rootItem->appendChild(packageItem);

    auto models = findModelsInPath(dir.absolutePath());
    for(const auto &model : models) {
        auto modelItem = new ModelTreeItem{model.first, model.second, packageItem};
        packageItem->appendChild(modelItem);
    }
}

QVector<QPair<QString, QString>> ModelTreeModel::findModelsInPath(QString directory) {
    QVector<QPair<QString, QString>> models;

    QDirIterator iterator{directory, QStringList{} << "*.config", QDir::Files, QDirIterator::Subdirectories};

    while(iterator.hasNext()) {
        auto filepath = iterator.next();

        QFile configFile{filepath};
        if(!configFile.open(QIODevice::ReadOnly | QIODevice::Text))
            continue;

        QDomDocument modelConfig;
        modelConfig.setContent(configFile.readAll());
        configFile.close();

        if(modelConfig.documentElement().tagName() != tr("model"))
            continue;

        auto name = modelConfig.documentElement().firstChildElement("name").text();

        QFileInfo configFileInfo{configFile};
        QDir configPath = configFileInfo.absoluteDir();

        auto sdfName = configPath.dirName();

        models.append({name, sdfName});
    }

    return models;
}
