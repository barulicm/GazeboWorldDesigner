#ifndef GWD_WORLD_H
#define GWD_WORLD_H

#include <vector>
#include <memory>
#include "SDFElement.h"
#include "Scene.h"

class World : public SDFElement {

public:
    std::string name = "default";

    Scene scene;

    std::vector<std::unique_ptr<SDFElement>> elements;

    QDomElement toXML(QDomDocument &document) const override;
};


#endif //GWD_WORLD_H
