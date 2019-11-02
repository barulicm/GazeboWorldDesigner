#ifndef GWD_MAINWINDOW_H
#define GWD_MAINWINDOW_H

#include <QMainWindow>
#include "WorldView.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

public slots:

private:
    void createMenus();
    [[nodiscard]] QWidget* createLeftPanel();
    [[nodiscard]] QWidget* createRightPanel();

    WorldView worldView;

};


#endif //GWD_MAINWINDOW_H
