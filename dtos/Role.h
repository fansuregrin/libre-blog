#pragma once

#include <string>
#include <json/json.h>

struct Role {
    static const int ADMINISTRATOR;
    static const int EDITOR;
    static const int CONTRIBUTOR;
    static const int SUBSCRIBER;
    
    int id = -1;
    std::string name;
    int menu = -1;

    Json::Value toJson() const;
};