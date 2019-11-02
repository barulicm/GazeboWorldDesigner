#ifndef GWD_MAINWINDOW_H
#define GWD_MAINWINDOW_H

#include <QMainWindow>
#include "FileManager.h"
#include "WorldView.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

public slots:

private:
    void createMenus(FileManager *fileManager);
    [[nodiscard]] QWidget* createLeftPanel();
    [[nodiscard]] QWidget *
    createRightPanel(FileManager *fileManager, WorldView *worldView);

};


#endif //GWD_MAINWINDOW_H
