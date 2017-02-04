#ifndef GWD_PROPERTYPANE_H
#define GWD_PROPERTYPANE_H

#include <QWidget>
#include <QFormLayout>
#include <map>
#include "sdf/SDFElement.h"

class PropertyPane : public QWidget {
Q_OBJECT
public:
    PropertyPane(QWidget *parent = nullptr);


public slots:
    void showProperties(SDFElement *element);

private:

    QFormLayout *layout;

    void clearChildren();

};


#endif //GWD_PROPERTYPANE_H
