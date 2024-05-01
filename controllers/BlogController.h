#pragma once

#include <drogon/HttpController.h>
#include "../models/User.h"
#include "../models/Category.h"
#include "../models/Article.h"

using namespace drogon;
using orm::Mapper;
using drogon_model::dg_test::Article;

class BlogController : public drogon::HttpController<BlogController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(BlogController::articleList, "/blog/page/{page}", Get);
    ADD_METHOD_TO(BlogController::getArticle, "/blog/article/{id}", Get);
    ADD_METHOD_TO(BlogController::updateArticle, "/blog/article/update", Post);
    ADD_METHOD_TO(BlogController::deleteArticles, "/blog/article/delete", Post);
    ADD_METHOD_TO(BlogController::getCategories, "/blog/category", Get);
    METHOD_LIST_END

    void articleList(
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
        const Article &article
    ) const;

    void deleteArticles(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void getCategories(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
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

}