#ifndef GWD_MAINWINDOW_H
#define GWD_MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include "WorldView.h"
#include "PropertyPane.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

public slots:

private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QAction *closeAct;
    QAction *newFileAct;
    QAction *openFileAct;
    QAction *saveFileAct;
    QAction *saveFileAsAct;

    WorldView worldView;

    QTreeView modelTreeView;

    PropertyPane propertyPane;
    PropertyPane scenePropertyPane;

};


#endif //GWD_MAINWINDOW_H
