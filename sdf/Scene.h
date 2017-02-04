#ifndef GWD_SCENE_H
#define GWD_SCENE_H

#include <experimental/optional>
#include <algorithm>
#include "SDFElement.h"
#include "Color.h"

class Scene : public SDFElement {

public:

    QDomElement toXML(QDomDocument &document) const override;

    Color ambient = Color(1,1,1,1);
    Color background = Color(0.7,0.7,0.7,1);
    bool shadows = true;
    bool show_grid = true;
    bool show_origin = true;

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
