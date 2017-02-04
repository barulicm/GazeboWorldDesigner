#include "Parser.h"
#include <sstream>

using namespace std;

World Parser::ParseWorld(const QDomDocument &xml) {
    World world;

    auto worldElement = xml.documentElement().firstChildElement("world");

    if(worldElement.isNull())
        return world;

    world.scene = parseScene(worldElement.firstChildElement("scene"));

    auto children = worldElement.childNodes();

    for(int i = 0; i < children.size(); ++i) {
        auto child = children.item(i);
        if(child.toElement().tagName() == "include") {
            world.elements.push_back(parseInclude(children.item(i).toElement()));
        }
    }

    return world;
}

Scene Parser::parseScene(const QDomElement &sceneElement) {
    Scene scene;
    
    scene.ambient = Color::fromString(sceneElement.firstChildElement("ambient").text().toStdString());

    scene.background = Color::fromString(sceneElement.firstChildElement("background").text().toStdString());

    scene.shadows = boolFromString(sceneElement.firstChildElement("shadows").text().toStdString());

    auto gridElement = sceneElement.firstChildElement("grid");
    if(!gridElement.isNull()) {
        istringstream{gridElement.text().toStdString()} >> scene.show_grid;
    }

    auto originElement = sceneElement.firstChildElement("origin_visual");
    if(!originElement.isNull()) {
        istringstream{originElement.text().toStdString()} >> scene.show_origin;
    }

    auto skyElement = sceneElement.firstChildElement("sky");
    if(!skyElement.isNull()) {
        auto timeElement = skyElement.firstChildElement("time");
        if(!timeElement.isNull()) {
            scene.time = stod(timeElement.text().toStdString());
        }
        auto sunriseElement = skyElement.firstChildElement("sunrise");
        if(!sunriseElement.isNull()) {
            scene.sunrise_time = stod(sunriseElement.text().toStdString());
        }
        auto sunsetElement = skyElement.firstChildElement("sunset");
        if(!sunsetElement.isNull()) {
            scene.sunset_time = stod(sunsetElement.text().toStdString());
        }
        auto cloudsElement = skyElement.firstChildElement("clouds");
        if(!cloudsElement.isNull()) {
            auto speedElement = cloudsElement.firstChildElement("speed");
            if(!speedElement.isNull()) {
                scene.clouds_speed = stod(speedElement.text().toStdString());
            }
            auto directionElement = cloudsElement.firstChildElement("direction");
            if(!directionElement.isNull()) {
                scene.clouds_direction = stod(directionElement.text().toStdString());
            }
            auto humidityElement = cloudsElement.firstChildElement("humidity");
            if(!humidityElement.isNull()) {
                scene.clouds_humidity = stod(humidityElement.text().toStdString());
            }
            auto meanSizeElement = cloudsElement.firstChildElement("mean_size");
            if(!meanSizeElement.isNull()) {
                scene.clouds_mean_size = stod(meanSizeElement.text().toStdString());
            }
            auto ambientElement = cloudsElement.firstChildElement("ambient");
            if(!ambientElement.isNull()) {
                scene.clouds_ambient_color = Color::fromString(ambientElement.text().toStdString());
            }
        }
    }

    return scene;
}

unique_ptr<IncludedElement> Parser::parseInclude(const QDomElement &includeElement) {

    unique_ptr<IncludedElement> element{new IncludedElement};

    element->uri = includeElement.firstChildElement("uri").text().toStdString();

    auto nameElement = includeElement.firstChildElement("name");
    if(!nameElement.isNull()) {
        element->name = nameElement.text().toStdString();
    }

    auto staticElement = includeElement.firstChildElement("static");
    if(!staticElement.isNull()) {
        bool tmp;
        istringstream{staticElement.text().toStdString()} >> tmp;
        element->isStatic = tmp;
    }

    auto poseElement = includeElement.firstChildElement("pose");
    if(!poseElement.isNull()) {
        element->pose = Pose::fromString(poseElement.text().toStdString());
    }

    return move(element);

}

bool Parser::boolFromString(const std::string &s) {

    string s_lower = s;
    transform(s.begin(), s.end(), s_lower.begin(), ::tolower);

    if(s_lower == "true" || s_lower == "1") {
        return true;
    }

    return false;
}
