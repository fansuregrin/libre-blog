#pragma once

#include <drogon/drogon.h>
#include "../models/Tag.h"
#include "../models/User.h"
#include "../models/Article.h"
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
using drogon_model::libre_blog::Tag;
using drogon_model::libre_blog::ArticleTag;

class TagController : public HttpController<TagController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(TagController::getAllTags, "/tags", HttpMethod::Get);
    ADD_METHOD_TO(TagController::tagList, "/tags/{page}", HttpMethod::Get);
    ADD_METHOD_TO(TagController::getTag, "/tag/{id}", HttpMethod::Get);
    ADD_METHOD_TO(TagController::addTag, "/admin/tag", HttpMethod::Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(TagController::updateTag, "/admin/tag", HttpMethod::Put, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(TagController::deleteTags, "/admin/tag", HttpMethod::Delete, "LoginFilter", "JsonFilter");
    METHOD_LIST_END

    void getAllTags(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
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
};