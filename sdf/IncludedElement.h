#ifndef GWD_INCLUDEDMODEL_H
#define GWD_INCLUDEDMODEL_H


#include <experimental/optional>
#include "SDFElement.h"
#include "Pose.h"

struct IncludedElement : public SDFElement {

    std::string uri;
    std::experimental::optional<std::string> name;
    std::experimental::optional<bool> isStatic;
    std::experimental::optional<Pose> pose;

    QDomElement toXML(QDomDocument &document) const override;

    void render(QPainter &painter, const QPointF &origin, const double &scale, bool selected) const override;

    double distanceToPoint(const double &x, const double &y) const override;

    void setPose(const double &x, const double &y, const double &z) override;
};


#endif //GWD_INCLUDEDMODEL_H
