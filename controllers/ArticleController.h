#pragma once

#include <drogon/drogon.h>
#include "TransformRequest.h"
#include "../models/User.h"
#include "../models/Role.h"
#include "../models/Category.h"
#include "../models/Article.h"
#include "../models/Tag.h"
#include "../models/ArticleTag.h"
#include "../utils/Utils.h"
#include "../dtos/ApiResponse.h"
#include "../exceptions/PageException.h"
#include "../exceptions/PermissionException.h"

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

class ArticleController : public HttpController<ArticleController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(ArticleController::articleList, "/articles/{page}", HttpMethod::Get);
    ADD_METHOD_TO(ArticleController::articleListAdmin, "/admin/articles/{page}", HttpMethod::Get, "LoginFilter");
    ADD_METHOD_TO(ArticleController::articleListByCategory, "/category/{slug}/{page}", HttpMethod::Get);
    ADD_METHOD_TO(ArticleController::articleListByTag, "/tag/{slug}/{page}", HttpMethod::Get);
    ADD_METHOD_TO(ArticleController::articleListByAuthor, "/user/{id}/{page}", HttpMethod::Get);
    ADD_METHOD_TO(ArticleController::getArticle, "/article/{id}", HttpMethod::Get);
    ADD_METHOD_TO(ArticleController::addArticle, "/admin/article", HttpMethod::Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(ArticleController::updateArticle, "/admin/article", HttpMethod::Put, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(ArticleController::deleteArticles, "/admin/article", HttpMethod::Delete, "LoginFilter", "JsonFilter");
    METHOD_LIST_END

    void articleList(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int page = 1
    ) const;

    void articleListAdmin(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int page = 1
    ) const;

    void articleListByCategory(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const std::string &slug,
        int page = 1
    ) const;

    void articleListByAuthor(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int id,
        int page = 1
    ) const;

    void articleListByTag(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const std::string &slug,
        int page = 1
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
};