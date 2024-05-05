#pragma once

#include <drogon/HttpController.h>
#include "../models/User.h"
#include "../models/Category.h"
#include "../models/Article.h"
#include "../models/Tag.h"
#include "../models/ArticleTag.h"

using namespace drogon;
using orm::Mapper;
using drogon_model::dg_test::Article;

class BlogController : public drogon::HttpController<BlogController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(BlogController::articleList, "/blog/page/{page}", Get);
    ADD_METHOD_TO(BlogController::articleListAdmin, "/blog/admin/page/{page}", Get);
    ADD_METHOD_TO(BlogController::getArticle, "/blog/article/{id}", Get);
    ADD_METHOD_TO(BlogController::updateArticle, "/blog/article/update", Post);
    ADD_METHOD_TO(BlogController::deleteArticles, "/blog/article/delete", Post);
    ADD_METHOD_TO(BlogController::getCategories, "/blog/category", Get);
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
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr) {
        throw std::invalid_argument("请求体格式错误, 请使用json");
    }
    
    auto &json = *jsonPtr;
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

}