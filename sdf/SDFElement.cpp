#include <QtXml/QDomElement>
#include "SDFElement.h"

using namespace std;

QDomElement SDFElement::toXML(QDomDocument &document) const {
    return QDomElement{};
}

template<>
QDomElement SDFElement::optionalToXML(QDomDocument &document, const std::experimental::optional<std::string> &optional) const {

    if(optional) {
        QDomElement element = document.createElement(typeid(std::string).name());
        element.appendChild(document.createTextNode(optional.value().c_str()));
        return element;
    } else {
        return QDomElement{};
    }
}

template<>
QDomElement SDFElement::optionalToXML(QDomDocument &document, const std::experimental::optional<bool> &optional) const {
    if(optional) {
        QDomElement element = document.createElement(typeid(std::string).name());
        element.appendChild(document.createTextNode( (optional.value() ? "true" : "false") ));
        return element;
    } else {
        return QDomElement{};
    }
}

void SDFElement::render(QPainter &, const QPointF &, const double &) const {

}

double SDFElement::distanceToPoint(const double &x, const double &y) const {
    return numeric_limits<double>::infinity();
}

void SDFElement::setPose(const double &x, const double &y, const double &z) {

}
