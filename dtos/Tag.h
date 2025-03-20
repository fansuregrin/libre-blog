#pragma once

#include <string>
#include <json/json.h>

struct Tag {
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

using TagPtr = std::shared_ptr<Tag>;