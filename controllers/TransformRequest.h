#pragma once

#include <drogon/drogon.h>
#include <json/json.h>
#include "../models/Category.h"
#include "../models/Article.h"
#include "../models/Tag.h"
#include "../models/User.h"
#include "../dtos/Password.h"

using namespace drogon;
using orm::Mapper;
using drogon_model::libre_blog::Article;
using drogon_model::libre_blog::Category;
using drogon_model::libre_blog::Tag;
using drogon_model::libre_blog::User;

namespace drogon {

template <>
inline Article fromRequest(const HttpRequest &req) {
    auto &json = *req.getJsonObject();
    if (!json.isMember("id") || json["id"].type() != Json::intValue) {
        throw std::invalid_argument("缺少必备字段: id, 或者类型错误");
    }
    if (!json.isMember("author") || json["author"].type() != Json::intValue) {
        throw std::invalid_argument("缺少必备字段: author, 或者类型错误");
    }
    if (!json.isMember("category") || json["category"].type() != Json::intValue) {
        throw std::invalid_argument("缺少必备字段: category, 或者类型错误");
    }
    
    Article article;
    article.updateByJson(json);
    return article;
}

template <>
inline std::vector<std::string> fromRequest(const HttpRequest &req) {
    std::vector<std::string> tags;
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr) {
        return tags;
    }
    
    auto &json = *jsonPtr;
    if (!json.isMember("tags") || json["tags"].type() != Json::arrayValue) {
        return tags;
    }

    for (const auto &tag : json["tags"]) {
        tags.emplace_back(tag.asString());
    }

    return tags;
}

template <>
inline Category fromRequest(const HttpRequest &req) {
    auto &json = *req.getJsonObject();
    if (!json.isMember("id") || json["id"].type() != Json::intValue) {
        throw std::invalid_argument("缺少必备字段: id, 或者类型错误");
    }
    if (!json.isMember("name") || json["name"].type() != Json::stringValue) {
        throw std::invalid_argument("缺少必备字段: name, 或者类型错误");
    }
    if (!json.isMember("slug") || json["slug"].type() != Json::stringValue) {
        throw std::invalid_argument("缺少必备字段: slug, 或者类型错误");
    }

    Category cat;
    cat.updateByJson(json);
    return cat;
}

template <>
inline Tag fromRequest(const HttpRequest &req) {
    auto &json = *req.getJsonObject();
    if (!json.isMember("id") || json["id"].type() != Json::intValue) {
        throw std::invalid_argument("缺少必备字段: id, 或者类型错误");
    }
    if (!json.isMember("name") || json["name"].type() != Json::stringValue) {
        throw std::invalid_argument("缺少必备字段: name, 或者类型错误");
    }
    if (!json.isMember("slug") || json["slug"].type() != Json::stringValue) {
        throw std::invalid_argument("缺少必备字段: slug, 或者类型错误");
    }

    Tag tag;
    tag.updateByJson(json);
    return tag;
}

template <>
inline User fromRequest(const HttpRequest &req) {
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr) {
        throw std::invalid_argument("请求体格式错误, 请使用json");
    }
    auto &json = *jsonPtr;
    if (!json.isMember("username") || 
    json["username"].type() != Json::stringValue) {
        throw std::invalid_argument("缺少必备字段: username, 或者类型错误");
    }
    if (!json.isMember("password") || 
    json["password"].type() != Json::stringValue) {
        throw std::invalid_argument("缺少必备字段: password, 或者类型错误");
    }
    User user;
    user.updateByJson(json);
    if (user.getValueOfRealname().empty()) {
        user.setRealname(user.getValueOfUsername());
    }
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
