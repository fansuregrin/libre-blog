#pragma once

#include <string>
#include <vector>
#include <json/json.h>
#include "../utils/Utils.h"

struct Menu {
    int id;
    std::string label;
    std::string name;
    int parent;
    std::vector<int> ancestor;

    Json::Value toJson() const;
};