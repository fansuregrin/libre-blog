#pragma once

#include <string>
#include <json/json.h>

struct Role {
    static const int ADMINISTRATOR = 1;
    static const int EDITOR = 2;
    static const int CONTRIBUTOR = 3;
    static const int SUBSCRIBER = 4;
    
    int id = -1;
    std::string name;
    int menu = -1;

    Role() {}

    Role(int id, const std::string &name, int menu) : 
        id(id), name(name), menu(menu) {}

    Role(const Role &o) : 
        id(o.id), name(o.name), menu(o.menu) {}

    Json::Value toJson() const {
        Json::Value r;
        r["id"] = id;
        r["name"] = name;
        r["menu"] = menu;
        return r;
    }
};