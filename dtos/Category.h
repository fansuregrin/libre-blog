#pragma once

#include <string>
#include <json/json.h>

struct Category {
    static const int UNCATEGORIED;

    int id = -1;
    std::string slug;
    std::string name;
    std::string createTime;
    std::string modifyTime;

    Json::Value toJson() const;
};

using CategoryPtr = std::shared_ptr<Category>;