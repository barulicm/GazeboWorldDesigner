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
}

void WorldView::paintEvent(QPaintEvent *) {

    QPainter painter{this};

    drawOrigin(painter);

    QPointF origin{width()/2.0, height()/2.0};

    for(size_t i = 0; i < world.elements.size(); ++i) {
        world.elements[i]->render(painter, origin, scale, (i == currentlySelectedIndex));
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
    const QMimeData *mimeData = event->mimeData();
    if(mimeData->hasText()) {
        std::unique_ptr<IncludedElement> element = std::make_unique<IncludedElement>();
        element->uri = "model://" + mimeData->text().toStdString();
        element->isStatic = true;

        const QPointF &dropPos = event->posF();
        element->pose = Pose{};
        element->pose->x = (dropPos.x() -  width()/2.0) * scale;
        element->pose->y = (dropPos.y() - height()/2.0) * scale;
        element->pose->z = 0.0;

        world.elements.push_back(std::move(element));

        currentlySelectedIndex = world.elements.size()-1;
        showProperties(world.elements[currentlySelectedIndex].get());
        update();
    }
    event->acceptProposedAction();
    update();
}

void WorldView::wheelEvent(QWheelEvent *event) {
    auto delta = event->angleDelta().y()/120;

    scale += delta * 0.001;

    scale = std::max(0.00000001, std::min(scale, 1.0));

    event->accept();

    update();
}

void WorldView::mousePressEvent(QMouseEvent *event) {
    if((event->buttons() | Qt::MouseButton::LeftButton) != 0 && !world.elements.empty()) {
        // check if close enough to an object
        QPointF origin{width()/2.0, height()/2.0};
        QPointF pressPos = (event->pos() - origin) * scale;
        std::vector<double> distances(world.elements.size(), 0);
        std::transform(world.elements.begin(), world.elements.end(), distances.begin(), [&pressPos,this](const std::unique_ptr<SDFElement> &e){
            return e->distanceToPoint(pressPos.x(), pressPos.y()) / scale;
        });
        auto mindist = std::min_element(distances.begin(), distances.end());
        if(*mindist < 10) {
            size_t index = static_cast<size_t>(std::distance(distances.begin(), mindist));
            if(index != currentlySelectedIndex) {
                currentlySelectedIndex = static_cast<size_t>(std::distance(distances.begin(), mindist));
                emit showProperties(world.elements[currentlySelectedIndex].get());
            }
            isDragging = true;
        }
    }
}

void WorldView::mouseMoveEvent(QMouseEvent *event) {
    if(isDragging) {
        const auto &draggedElement = world.elements[currentlySelectedIndex];
        QPointF origin{width()/2.0, height()/2.0};
        QPointF movePos = (event->pos() - origin) * scale;
        draggedElement->setPose(movePos.x(), movePos.y(), 0);
        update();
    }
}

void WorldView::mouseReleaseEvent(QMouseEvent *event) {
    if((event->buttons() | Qt::MouseButton::LeftButton) != 0) {
        isDragging = false;
        update();
    }
}

void WorldView::newFile() {
    world = World{};
    currentFilePath.clear();
    update();
}

void WorldView::openFile() {
    auto filename = QFileDialog::getOpenFileName(this, "Open World",
                                                 QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                 "SDF Files (*.sdf *.world *.xml)");
    if(filename.isEmpty())
        return;

    QFile file{filename};
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Gazebo World Designer", "Could not open world file\n" + filename, QMessageBox::Ok);
        return;
    }

    QDomDocument document;
    document.setContent(file.readAll());
    file.close();


    world = Parser::ParseWorld(document);

    currentFilePath = filename.toStdString();

    update();
}

void WorldView::saveFile() {
    if(currentFilePath.empty()) {
        saveFileAs();
    } else {
        writeWorldToFile(currentFilePath);
    }
}

void WorldView::saveFileAs() {
    currentFilePath = QFileDialog::getSaveFileName(this, "Save World",
                                                   QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
                                                   "SDF Files (*.sdf *.world *.xml)").toStdString();

    if(currentFilePath.empty()) // Dialog cancelled
        return;

    writeWorldToFile(currentFilePath);
}

void WorldView::writeWorldToFile(const std::string &filepath) {

    QDomDocument xmlDocument;
    auto xml = world.toXML(xmlDocument);

    QDomElement sdfElement = xmlDocument.createElement("sdf");
    sdfElement.setAttribute("version", "1.4");
    sdfElement.appendChild(xml);
    xmlDocument.appendChild(sdfElement);

    QFile file{filepath.c_str()};
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this, "Gazebo World Designer",
                              ("Unable to open file for saving: " + currentFilePath).c_str(), QMessageBox::Ok);
        return;
    }
    QTextStream textStream{&file};
    xmlDocument.save(textStream,1);
    textStream.flush();
    file.close();
}

void WorldView::drawOrigin(QPainter &painter) {
    QPointF c{width()/2.0, height()/2.0};
    painter.setPen(Qt::red);
    painter.drawLine(c, c + QPointF{1.0/scale, 0});
    painter.setPen(Qt::green);
    painter.drawLine(c, c + QPointF{0, 1.0/scale});
}
