#pragma once

#include <drogon/HttpController.h>
#include "../models/User.h"
#include "../models/Category.h"
#include "../models/Article.h"
#include "../models/Tag.h"
#include "../models/ArticleTag.h"

using namespace drogon;
using orm::Mapper;
using orm::Criteria;
using orm::CompareOperator;
using orm::SortOrder;
using drogon_model::dg_test::Article;
using drogon_model::dg_test::User;
using drogon_model::dg_test::Category;
using drogon_model::dg_test::Tag;
using drogon_model::dg_test::ArticleTag;

class BlogController : public drogon::HttpController<BlogController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(BlogController::articleList, "/blog/page/{page}", Get);
    ADD_METHOD_TO(BlogController::articleListAdmin, "/blog/admin/page/{page}", Get, "LoginFilter");
    ADD_METHOD_TO(BlogController::getArticle, "/blog/article/{id}", Get);
    ADD_METHOD_TO(BlogController::addArticle, "/blog/article/add", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(BlogController::updateArticle, "/blog/article/update", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(BlogController::deleteArticles, "/blog/article/delete", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(BlogController::getCategories, "/blog/category", Get);
    ADD_METHOD_TO(BlogController::updateCategory, "/blog/category/update", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(BlogController::articleListByCategory, "/blog/category/{slug}/{page}", Get);
    ADD_METHOD_TO(BlogController::articleListByAuthor, "/blog/user/{id}/{page}", Get);
    ADD_METHOD_TO(BlogController::articleListByTag, "/blog/tag/{slug}/{page}", Get);
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

    void updateCategory(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const Category &cat
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

}