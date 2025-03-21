#include "Category.h"

const int Category::UNCATEGORIED = 1;

Json::Value Category::toJson() const {
    Json::Value r;
    r["id"] = id;
    r["slug"] = slug;
    r["name"] = name;
    r["createTime"] = createTime;
    r["modifyTime"] = modifyTime;
    return r;
}