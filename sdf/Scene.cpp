#include "Scene.h"

using namespace std;

QDomElement Scene::toXML(QDomDocument &document) const {

    QDomElement xml = document.createElement("scene");

    auto ambientElement = ambient.toXML(document);
    ambientElement.setTagName("ambient");
    xml.appendChild(ambientElement);

    auto backgroundElement = background.toXML(document);
    backgroundElement.setTagName("background");
    xml.appendChild(backgroundElement);

    QDomElement shadowsElement = document.createElement("shadows");
    shadowsElement.appendChild( document.createTextNode( (shadows ? "true" : "false") ) );
    xml.appendChild(shadowsElement);


    auto timeElement = optionalToXML(document, time);
    timeElement.setTagName("time");
    xml.appendChild(timeElement);

    auto sunriseElement = optionalToXML(document, sunrise_time);
    sunriseElement.setTagName("sunrise");
    xml.appendChild(sunriseElement);

    auto sunsetElement = optionalToXML(document, sunset_time);
    sunsetElement.setTagName("sunset");
    xml.appendChild(sunsetElement);

    if(clouds_ambient_color || clouds_humidity || clouds_mean_size || clouds_speed || clouds_direction) {
        auto cloudsElement = document.createElement("clouds");

        auto ambientColorElement = optionalToXML(document, clouds_ambient_color);
        ambientColorElement.setTagName("ambient");
        cloudsElement.appendChild(ambientColorElement);

        auto humidityElement = optionalToXML(document, clouds_humidity);
        humidityElement.setTagName("humidity");
        cloudsElement.appendChild(humidityElement);

        auto meanSizeElement = optionalToXML(document, clouds_mean_size);
        meanSizeElement.setTagName("mean_size");
        cloudsElement.appendChild(meanSizeElement);

        auto speedElement = optionalToXML(document, clouds_speed);
        speedElement.setTagName("speed");
        cloudsElement.appendChild(speedElement);

        auto directionElement = optionalToXML(document, clouds_direction);
        directionElement.setTagName("direction");
        cloudsElement.appendChild(directionElement);

        xml.appendChild(cloudsElement);
    }

    return xml;
}