#include <QtXml/QDomElement>
#include "IncludedElement.h"

using namespace std;

QDomElement IncludedElement::toXML(QDomDocument &document) const {
    QDomElement xml = document.createElement("include");

    QDomElement uriElement = document.createElement("uri");
    uriElement.appendChild(document.createTextNode(uri.c_str()));
    xml.appendChild(uriElement);

    auto nameElement = optionalToXML(document, name);
    nameElement.setTagName("name");
    xml.appendChild(nameElement);

    auto staticElement = optionalToXML(document, isStatic);
    staticElement.setTagName("static");
    xml.appendChild(staticElement);

    auto poseElement = optionalToXML(document, pose);
    poseElement.setTagName("pose");
    xml.appendChild(poseElement);

    return xml;
}

void IncludedElement::render(QPainter &painter, const QPointF &origin, const double &scale) const {
    QPointF posPoint;
    if(pose) {
        posPoint.setX(pose->x);
        posPoint.setY(pose->y);
    } else {
        posPoint.setX(0);
        posPoint.setY(0);
    }
    posPoint /= scale;
    posPoint += origin;
    painter.setPen(Qt::lightGray);
    painter.drawLine(posPoint + QPointF{-10,0}, posPoint + QPointF{10,0});
    painter.drawLine(posPoint + QPointF{0,-10}, posPoint + QPointF{0,10});

    painter.drawText(posPoint + QPointF{2,-2}, uri.c_str());
}

double IncludedElement::distanceToPoint(const double &x, const double &y) const {
    if(!pose)
        return std::numeric_limits<double>::infinity();

    double dx = x - pose->x;
    double dy = y - pose->y;
    return std::sqrt( (dx*dx) + (dy*dy) );
}

void IncludedElement::setPose(const double &x, const double &y, const double &z) {
    if(pose) {
        pose->x = x;
        pose->y = y;
        pose->z = z;
    }
}
