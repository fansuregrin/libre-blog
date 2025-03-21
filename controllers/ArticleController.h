#pragma once

#include <drogon/drogon.h>
#include "TransformRequest.h"
#include "../utils/Utils.h"
#include "../dtos/Article.h"
#include "../dtos/Role.h"
#include "../dtos/ApiResponse.h"
#include "../mappers/ArticleMapper.h"
#include "../mappers/UserMapper.h"
#include "../mappers/TagMapper.h"
#include "../mappers/ArticleTagMapper.h"
#include "../exceptions/PermissionException.h"

using namespace drogon;
using orm::Result;

class ArticleController : public HttpController<ArticleController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ArticleController::articleList, "/articles", HttpMethod::Get, "PaginationFilter");
    ADD_METHOD_TO(ArticleController::articleListAdmin, "/admin/articles", HttpMethod::Get, "LoginFilter", "PaginationFilter");
    ADD_METHOD_TO(ArticleController::articleListByCategory, "/articles/category/{slug}", HttpMethod::Get, "PaginationFilter");
    ADD_METHOD_TO(ArticleController::articleListByTag, "/articles/tag/{slug}", HttpMethod::Get, "PaginationFilter");
    ADD_METHOD_TO(ArticleController::articleListByAuthor, "/articles/user/{id}", HttpMethod::Get, "PaginationFilter");
    ADD_METHOD_TO(ArticleController::getArticle, "/article/{id}", HttpMethod::Get);
    ADD_METHOD_TO(ArticleController::addArticle, "/admin/article", HttpMethod::Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(ArticleController::updateArticle, "/admin/article", HttpMethod::Put, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(ArticleController::deleteArticles, "/admin/article", HttpMethod::Delete, "LoginFilter", "JsonFilter");
    METHOD_LIST_END

    void articleList(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void articleListAdmin(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void articleListByCategory(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const std::string &slug
    ) const;

    void articleListByAuthor(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int id
    ) const;

    void articleListByTag(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const std::string &slug
    ) const;

    void getArticle(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int id
    ) const;

    void addArticle(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        Article article
    ) const;

    void updateArticle(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        Article article
    ) const;

    void deleteArticles(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;
private:
    static const std::string articleListSql;
    static const std::string articleCountSql;
    static const std::string articleListByUserSql;
    static const std::string articleCountByUserSql;
    static const std::string articleListByCategorySql;
    static const std::string articleCountByCategorySql;
    static const std::string articleListByTagSql;
    static const std::string articleCountByTagSql;
    static const std::string articleGetSql;
    static const std::string articleInsertSql;
};