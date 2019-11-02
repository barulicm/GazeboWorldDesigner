#include "WorldView.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QMimeData>
#include <QDropEvent>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTextStream>
#include <iostream>
#include "sdf/IncludedElement.h"
#include "sdf/Parser.h"

WorldView::WorldView(QWidget *parent) : QWidget(parent) {
    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
}

void WorldView::newWorld(World &newWorld) {
    world = &newWorld;
    setSelectedIndex(-1);
    update();
}

void WorldView::paintEvent(QPaintEvent *) {

    QPainter painter{this};

    drawOrigin(painter);

    if(world != nullptr) {
        for (size_t i = 0; i < world->elements.size(); ++i) {
            world->elements[i]->render(painter, scaledOrigin(), scale,
                                      (i == static_cast<size_t>(selectedIndex)));
        }
    }
}

void WorldView::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}

void WorldView::dragMoveEvent(QDragMoveEvent *event) {
    event->acceptProposedAction();
}

void WorldView::dragLeaveEvent(QDragLeaveEvent *event) {
    event->accept();
}

void WorldView::dropEvent(QDropEvent *event) {
    if(world != nullptr) {
        const QMimeData *mimeData = event->mimeData();
        if (mimeData->hasText()) {
            std::unique_ptr<IncludedElement> element = std::make_unique<IncludedElement>();
            element->uri = "model://" + mimeData->text().toStdString();
            element->isStatic = true;

            const QPointF &dropPos = event->posF();
            element->pose = Pose{};
            element->pose->x = (dropPos.x() - scaledOrigin().x()) * scale;
            element->pose->y = (dropPos.y() - scaledOrigin().y()) * scale;
            element->pose->z = 0.0;

            world->elements.push_back(std::move(element));

            setSelectedIndex(static_cast<int>(world->elements.size() - 1));
            update();
        }
        setFocus();
        event->acceptProposedAction();
        update();
    }
}

void WorldView::wheelEvent(QWheelEvent *event) {
    auto delta = event->angleDelta().y()/120;

    scale += delta * 0.001;

    scale = std::max(0.00000001, std::min(scale, 1.0));

    event->accept();

    update();
}

void WorldView::mousePressEvent(QMouseEvent *event) {
    if(world != nullptr) {
        if ((event->buttons() | Qt::MouseButton::LeftButton) != 0) {
            if (!world->elements.empty()) {
                // check if close enough to an object
                QPointF pressPos = (event->pos() - scaledOrigin()) * scale;
                std::vector<double> distances(world->elements.size(), 0);
                std::transform(world->elements.begin(), world->elements.end(),
                               distances.begin(), [&pressPos, this](
                        const std::unique_ptr<SDFElement> &e) {
                      return e->distanceToPoint(pressPos.x(), pressPos.y()) /
                             scale;
                    });
                auto mindist = std::min_element(distances.begin(),
                                                distances.end());
                if (*mindist < 10) {
                    auto index = static_cast<int>(std::distance(
                        distances.begin(), mindist));
                    if (index != selectedIndex) {
                        setSelectedIndex(
                            static_cast<size_t>(std::distance(distances.begin(),
                                                              mindist)));
                    }
                    isDraggingElement = true;
                } else {
                    isDraggingView = true;
                    prevMousePose = event->pos();
                }
            } else {
                isDraggingView = true;
                prevMousePose = event->pos();
            }
        }
    }
}

void WorldView::mouseMoveEvent(QMouseEvent *event) {
    if(world != nullptr) {
        if (isDraggingElement && selectedIndex >= 0) {
            const auto &draggedElement = world->elements[static_cast<size_t>(selectedIndex)];
            QPointF movePos = (event->pos() - scaledOrigin()) * scale;
            draggedElement->setPose(movePos.x(), movePos.y(), 0);
            update();
        } else if(isDraggingView) {
            auto deltaX = static_cast<float>(event->x() - prevMousePose.x()) / width();
            auto deltaY = static_cast<float>(event->y() - prevMousePose.y()) / height();
            origin.setX(origin.x() + deltaX);
            origin.setY(origin.y() + deltaY);
            prevMousePose = event->pos();
            update();
        }
    }
}

void WorldView::mouseReleaseEvent(QMouseEvent *event) {
    if((event->buttons() | Qt::MouseButton::LeftButton) != 0) {
        isDraggingElement = false;
        isDraggingView = false;
        update();
    }
}

void WorldView::keyPressEvent(QKeyEvent *event) {
    if(world != nullptr) {
        if (event->key() == Qt::Key_Delete && !world->elements.empty()) {
            world->elements.erase(
                world->elements.begin() + static_cast<int>(selectedIndex));
            setSelectedIndex(0);
            update();
        } else {
            QWidget::keyPressEvent(event);
        }
    }
}

void WorldView::drawOrigin(QPainter &painter) {
    QPointF c = scaledOrigin();
    painter.setPen(Qt::red);
    painter.drawLine(c, c + QPointF{1.0/scale, 0});
    painter.setPen(Qt::green);
    painter.drawLine(c, c + QPointF{0, 1.0/scale});
}

QPointF WorldView::scaledOrigin() {
    return QPointF{origin.x() * width(), origin.y() * height()};
}

void WorldView::setSelectedIndex(int index) {
    selectedIndex = index;
    emit onSelectedIndexChanged(selectedIndex);
}
