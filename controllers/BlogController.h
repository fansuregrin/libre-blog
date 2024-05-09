#pragma once

#include <drogon/HttpController.h>
#include "../models/User.h"
#include "../models/Role.h"
#include "../models/Category.h"
#include "../models/Article.h"
#include "../models/Tag.h"
#include "../models/ArticleTag.h"
#include "../models/Menu.h"

using namespace drogon;
using orm::Mapper;
using orm::Criteria;
using orm::CompareOperator;
using orm::SortOrder;
using drogon_model::libre_blog::Article;
using drogon_model::libre_blog::User;
using drogon_model::libre_blog::Role;
using drogon_model::libre_blog::Category;
using drogon_model::libre_blog::Tag;
using drogon_model::libre_blog::ArticleTag;
using drogon_model::libre_blog::Menu;

class BlogController : public drogon::HttpController<BlogController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(BlogController::articleList, "/blog/articles/{page}", Get);
    ADD_METHOD_TO(BlogController::articleListAdmin, "/blog/admin/articles/{page}", Get, "LoginFilter");
    ADD_METHOD_TO(BlogController::getArticle, "/blog/article/{id}", Get);
    ADD_METHOD_TO(BlogController::addArticle, "/blog/article/add", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(BlogController::updateArticle, "/blog/article/update", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(BlogController::deleteArticles, "/blog/article/delete", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(BlogController::articleListByCategory, "/blog/category/{slug}/{page}", Get);
    ADD_METHOD_TO(BlogController::getCategories, "/blog/categories", Get);
    ADD_METHOD_TO(BlogController::getCategory, "/blog/category/{id}", Get);
    ADD_METHOD_TO(BlogController::addCategory, "/blog/category/add", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(BlogController::updateCategory, "/blog/category/update", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(BlogController::deleteCategories, "/blog/category/delete", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(BlogController::articleListByTag, "/blog/tag/{slug}/{page}", Get);
    ADD_METHOD_TO(BlogController::tagList, "/blog/tags/{page}", Get);
    ADD_METHOD_TO(BlogController::getTag, "/blog/tag/{id}", Get);
    ADD_METHOD_TO(BlogController::addTag, "/blog/tag/add", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(BlogController::updateTag, "/blog/tag/update", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(BlogController::deleteTags, "/blog/tag/delete", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(BlogController::articleListByAuthor, "/blog/user/{id}/{page}", Get);
    ADD_METHOD_TO(BlogController::getMenuAdmin, "/blog/admin/menu", Get, "LoginFilter");
    ADD_METHOD_TO(BlogController::getMenu, "/blog/menu", Get);
    METHOD_LIST_END

    void articleList(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int page
    ) const;

    void articleListAdmin(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int page
    ) const;

    void getArticle(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int id
    ) const;

    void addArticle(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const std::vector<std::string> &tags
    ) const;

    void updateArticle(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const Article &article,
        const std::vector<std::string> &tags
    ) const;

    void deleteArticles(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void getCategories(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void getCategory(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int id
    ) const;

    void addCategory(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void updateCategory(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const Category &cat
    ) const;

    void deleteCategories(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void articleListByCategory(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const std::string &slug,
        int page
    ) const;

    void articleListByAuthor(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int id,
        int page
    ) const;

    void articleListByTag(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const std::string &slug,
        int page
    ) const;

    void tagList(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int page
    ) const;

    void getTag(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int id
    ) const;

    void addTag(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void updateTag(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const Tag &tag
    ) const;

    void deleteTags(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void getMenuAdmin(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void getMenu(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;
};


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

}

Json::Value generateMenu(Mapper<Menu> &mp, int32_t id);