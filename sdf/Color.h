#ifndef GWD_COLOR_H
#define GWD_COLOR_H

#include <QtXml/QDomElement>
#include "SDFElement.h"

struct Color : SDFElement {

    double r = 0;
    double g = 0;
    double b = 0;
    double a = 0;

    QDomElement toXML(QDomDocument &document) const override;

    static Color fromString(const std::string &s);

};

template<>
QDomElement SDFElement::optionalToXML(QDomDocument &document, const std::experimental::optional<Color> &optional) const;

#endif //GWD_COLOR_H
