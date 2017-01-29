//
// Created by matt on 1/27/17.
//

#ifndef GWD_SCENE_H
#define GWD_SCENE_H

#include <experimental/optional>
#include <algorithm>
#include "SDFElement.h"
#include "Color.h"

class Scene : SDFElement {

public:

    QDomElement toXML(QDomDocument &document) const override;

    Color ambient;
    Color background;
    bool shadows;
    bool show_grid;
    bool show_origin;

    std::experimental::optional<double> time;
    std::experimental::optional<double> sunrise_time;
    std::experimental::optional<double> sunset_time;
    std::experimental::optional<double> clouds_speed;
    std::experimental::optional<double> clouds_direction;
    std::experimental::optional<double> clouds_humidity;
    std::experimental::optional<double> clouds_mean_size;
    std::experimental::optional<Color> clouds_ambient_color;

};


#endif //GWD_SCENE_H
