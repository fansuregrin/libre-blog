#pragma once

#include <string>
#include <json/json.h>

struct Category {
    static const int UNCATEGORIED = 1;

    int id = -1;
    std::string slug;
    std::string name;
    std::string createTime;
    std::string modifyTime;

    Json::Value toJson() const {
        Json::Value r;
        r["id"] = id;
        r["slug"] = slug;
        r["name"] = name;
        r["createTime"] = createTime;
        r["modifyTime"] = modifyTime;
        return r;
    }
};

using CategoryPtr = std::shared_ptr<Category>;