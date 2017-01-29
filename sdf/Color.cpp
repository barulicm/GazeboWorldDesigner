#include "Color.h"
#include <sstream>

using namespace std;

QDomElement Color::toXML(QDomDocument &document) const {
    QDomElement element = document.createElement("color");
    element.appendChild(document.createTextNode((to_string(r) + " " + to_string(g) + " " + to_string(b) + " " + to_string(a)).c_str()));
    return element;
}

Color Color::fromString(const std::string &s) {

    stringstream ss{s};

    Color color;

    ss >> color.r;
    ss >> color.g;
    ss >> color.b;
    ss >> color.a;

    return color;
}

template<>
QDomElement SDFElement::optionalToXML(QDomDocument &document, const std::experimental::optional<Color> &optional) const {
    if(optional) {
        return optional->toXML(document);
    } else {
        return QDomElement{};
    }
}
