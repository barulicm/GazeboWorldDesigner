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
    void newWorld(World &newWorld);

signals:
    void onSelectedIndexChanged(int index);

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

    void keyPressEvent(QKeyEvent *event) override;

private:
    World *world = nullptr;

    void drawOrigin(QPainter &painter);

    double scale = 0.01; // m/px

    bool isDragging = false;

    int selectedIndex = 0;
    void setSelectedIndex(int index);

};

#endif //GWD_WORLDVIEW_H
