#pragma once

#include <drogon/drogon.h>
#include <json/json.h>
#include "../dtos/Password.h"
#include "../dtos/Article.h"

namespace drogon {

template <>
inline Article fromRequest(const HttpRequest &req) {
    auto &reqData = *req.getJsonObject();
    Article article;
    article.id = reqData.isMember("id") ? reqData["id"].asInt() : -1;
    article.title = reqData.isMember("title") ? reqData["title"].asString() : "";
    article.authorId = reqData.isMember("authorId") ? reqData["authorId"].asInt() : -1;
    article.categoryId = reqData.isMember("categoryId") ? reqData["categoryId"].asInt() : -1;
    article.excerpt = reqData.isMember("excerpt") ? reqData["excerpt"].asString() : "";
    article.content = reqData.isMember("content") ? reqData["content"].asString() : "";
    
    if (reqData.isMember("tags") && reqData["tags"].type() == Json::arrayValue) {
        for (const auto &t_ : reqData["tags"]) {
            Tag tag;
            tag.name = t_.asString();
            article.tags.emplace_back(tag);
        }
    }

    return article;
}

template <>
inline Category fromRequest(const HttpRequest &req) {
    auto &reqData = *req.getJsonObject();
    Category category;
    category.id = reqData.isMember("id") ? reqData["id"].asInt() : -1;
    category.slug = reqData.isMember("slug") ? reqData["slug"].asString() : "";
    category.name = reqData.isMember("name") ? reqData["name"].asString() : "";
    return category;
}

template <>
inline Tag fromRequest(const HttpRequest &req) {
    auto &reqData = *req.getJsonObject();
    Tag tag;
    tag.id = reqData.isMember("id") ? reqData["id"].asInt() : -1;
    tag.slug = reqData.isMember("slug") ? reqData["slug"].asString() : "";
    tag.name = reqData.isMember("name") ? reqData["name"].asString() : "";
    return tag;
}

template <>
inline User fromRequest(const HttpRequest &req) {
    auto &reqData = *req.getJsonObject();
    User user;
    user.id = reqData.isMember("id") ? reqData["id"].asInt() : -1;
    user.username = reqData.isMember("username") ? reqData["username"].asString() : "";
    user.realname = reqData.isMember("realname") ? reqData["realname"].asString() : "";
    user.password = reqData.isMember("password") ? reqData["password"].asString() : "";
    user.email = reqData.isMember("email") ? reqData["email"].asString() : "";
    user.createTime = reqData.isMember("creatTime") ? reqData["createTime"].asString() : "";
    user.role = reqData.isMember("role") ? reqData["role"].asInt() : -1;
    return user;
}

template <>
inline Password fromRequest(const HttpRequest &req) {
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr) {
        throw std::invalid_argument("请求体格式错误, 请使用json");
    }
    auto &json = *jsonPtr;
    if (!json.isMember("oldPassword") || json["oldPassword"].type() != Json::stringValue) {
        throw std::invalid_argument("缺失必备字段：oldPassword，或者类型错误");
    }
    if (!json.isMember("newPassword") || json["newPassword"].type() != Json::stringValue) {
        throw std::invalid_argument("缺失必备字段：newPassword，或者类型错误");        
    }
    Password password(json["oldPassword"].asString(), json["newPassword"].asString());
    return password;
}

}
