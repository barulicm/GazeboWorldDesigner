#ifndef GWD_PARSER_H
#define GWD_PARSER_H

#include "World.h"
#include "IncludedElement.h"
#include <QDomDocument>

class Parser {
public:
    static World ParseWorld(const QDomDocument &xml);

private:
    static Scene parseScene(const QDomElement &sceneElement);

    static std::unique_ptr<IncludedElement> parseInclude(const QDomElement &includeElement);

    static bool boolFromString(const std::string &s);
};


#endif //GWD_PARSER_H
