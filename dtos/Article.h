#pragma once

#include <string>
#include <vector>
#include <json/json.h>
#include "User.h"
#include "Category.h"
#include "Tag.h"
#include "../utils/Utils.h"

struct Article {
    int id = -1;
    std::string title;
    int authorId = -1;
    int categoryId = -1;
    std::string excerpt;
    std::string content;
    std::string createTime;

    User author;
    Category category;
    std::vector<Tag> tags;

    Json::Value toJson() const {
        Json::Value r;
        r["id"] = id;
        r["title"] = title;
        r["excerpt"] = excerpt;
        r["content"] = content;
        r["createTime"] = createTime;
        r["category"] = category.toJson();
        r["author"] = author.toJson();
        // r["tags"] = toJson(tags);
        return r;
    }
};

using ArticlePtr = std::shared_ptr<Article>;