
#include "World.h"

using namespace std;

QDomElement World::toXML(QDomDocument &document) const {
    QDomElement xml = document.createElement("world");
    xml.setAttribute("name", name.c_str());

    xml.appendChild(scene.toXML(document));

    for(const auto &element : elements) {
        xml.appendChild(element->toXML(document));
    }

    return xml;
}
