#include "Menu.h"

Json::Value Menu::toJson() const {
    Json::Value r;
    r["id"] = id;
    r["label"] = label;
    r["name"] = name;
    r["parent"] = parent;
    r["ancestor"] = ::toJson(ancestor);
    return r;
}