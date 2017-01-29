#ifndef GWD_WORLDVIEW_H
#define GWD_WORLDVIEW_H

#include <QWidget>
#include <QLabel>
#include "sdf/World.h"

class WorldView : public QWidget {
    Q_OBJECT

public:
    WorldView(QWidget *parent = nullptr);

public slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();

protected:
    void paintEvent(QPaintEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    World world;
    std::string currentFilePath;

    void writeWorldToFile(const std::string &filepath);

    void drawOrigin(QPainter &painter);

    double scale = 0.01; // m/px

    bool isDragging = false;

    size_t currentlyDraggingIndex = 0;

};

#endif //GWD_WORLDVIEW_H
