#include "Role.h"

const int Role::ADMINISTRATOR = 1;
const int Role::EDITOR = 2;
const int Role::CONTRIBUTOR = 3;
const int Role::SUBSCRIBER = 4;

Json::Value Role::toJson() const {
    Json::Value r;
    r["id"] = id;
    r["name"] = name;
    r["menu"] = menu;
    return r;
}