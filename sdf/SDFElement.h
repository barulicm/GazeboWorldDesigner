#ifndef GWD_SDFELEMENT_H
#define GWD_SDFELEMENT_H

#include <QPainter>
#include <string>
#include <experimental/optional>
#include <QDomElement>
#include <typeinfo>

class SDFElement {
public:
    virtual QDomElement toXML(QDomDocument &document) const;

    virtual void render(QPainter &painter, const QPointF &origin, const double &scale) const;

    virtual double distanceToPoint(const double &x, const double &y) const;

    virtual void setPose(const double &x, const double &y, const double &z);

protected:

    template<typename T>
    QDomElement optionalToXML(QDomDocument &document, const std::experimental::optional<T> &optional) const {
        if(optional) {
            QDomElement element = document.createElement(typeid(T).name());
            element.appendChild(document.createTextNode(std::to_string(optional.value()).c_str()));
            return element;
        } else {
            return QDomElement{};
        }
    }
};

template<>
QDomElement SDFElement::optionalToXML(QDomDocument &document, const std::experimental::optional<std::string> &optional) const;

template<>
QDomElement SDFElement::optionalToXML(QDomDocument &document, const std::experimental::optional<bool> &optional) const;

#endif //GWD_SDFELEMENT_H
