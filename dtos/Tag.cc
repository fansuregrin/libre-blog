#include "Tag.h"

Json::Value Tag::toJson() const {
    Json::Value r;
    r["id"] = id;
    r["slug"] = slug;
    r["name"] = name;
    r["createTime"] = createTime;
    r["modifyTime"] = modifyTime;
    return r;
}