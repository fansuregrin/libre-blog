#include "Article.h"

Json::Value Article::toJson() const {
    Json::Value r;
    r["id"] = id;
    r["title"] = title;
    r["excerpt"] = excerpt;
    r["content"] = content;
    r["createTime"] = createTime;
    r["modifyTime"] = modifyTime;
    r["category"] = category.toJson();
    r["author"] = author.toJson();
    r["tags"] = ::toJson(tags);
    return r;
}