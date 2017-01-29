#include "Pose.h"
#include <sstream>
#include <QtXml/QDomElement>

using namespace std;

QDomElement Pose::toXML(QDomDocument &document) const {
    QDomElement element = document.createElement("pose");
    element.appendChild(document.createTextNode((to_string(x) + " " + to_string(y) + " " + to_string(z) + " "
                                                 + to_string(roll) + " " + to_string(pitch) + " "
                                                 + to_string(yaw)).c_str()));
    return element;
}

Pose Pose::fromString(const std::string &s) {
    stringstream ss{s};

    Pose pose;

    ss >> pose.x;
    ss >> pose.y;
    ss >> pose.z;
    ss >> pose.roll;
    ss >> pose.pitch;
    ss >> pose.yaw;

    return pose;
}

template<>
QDomElement SDFElement::optionalToXML(QDomDocument &document, const std::experimental::optional<Pose> &optional) const {
    if(optional) {
        return optional->toXML(document);
    } else {
        return QDomElement{};
    }
}
