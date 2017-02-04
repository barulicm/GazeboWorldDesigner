#ifndef GWD_POSE_H
#define GWD_POSE_H

#include <QtXml/QDomElement>
#include "SDFElement.h"

struct Pose : public SDFElement {

    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double roll = 0.0;
    double pitch = 0.0;
    double yaw = 0.0;

    QDomElement toXML(QDomDocument &document) const override;

    static Pose fromString(const std::string &s);

};

template<>
QDomElement SDFElement::optionalToXML(QDomDocument &document, const std::experimental::optional<Pose> &optional) const;

#endif //GWD_POSE_H
