#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <iostream>
#include "MainWindow.h"

void loadStylesheet(QApplication &application) {
    QFile f{":qdarkstyle/style.qss"};
    if (!f.exists()) {
        std::cerr << "Unable to set stylesheet. File not found!" << std::endl;
    } else {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts{&f};
        application.setStyleSheet(ts.readAll());
    }
}

int main(int argc, char **argv) {

    QApplication application{argc, argv};

    loadStylesheet(application);

    MainWindow window;
    window.show();

    return application.exec();
}