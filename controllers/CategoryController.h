#pragma once

#include <drogon/drogon.h>
#include "TransformRequest.h"
#include "../models/User.h"
#include "../models/Role.h"
#include "../models/Category.h"
#include "../models/Article.h"
#include "../utils/Utils.h"
#include "../dtos/ApiResponse.h"
#include "../exceptions/PermissionException.h"

using namespace drogon;
using orm::Mapper;
using orm::Criteria;
using orm::CompareOperator;
using orm::SortOrder;
using drogon_model::libre_blog::Category;
using drogon_model::libre_blog::User;
using drogon_model::libre_blog::Role;
using drogon_model::libre_blog::Article;

class CategoryController : public HttpController<CategoryController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(CategoryController::getAllCategories, "/categorys", HttpMethod::Get);
    ADD_METHOD_TO(CategoryController::getCategory, "/category/{id}", HttpMethod::Get);
    ADD_METHOD_TO(CategoryController::addCategory, "/admin/category", HttpMethod::Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(CategoryController::updateCategory, "/admin/category", HttpMethod::Put, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(CategoryController::deleteCategories, "/admin/category", HttpMethod::Delete, "LoginFilter", "JsonFilter");
    METHOD_LIST_END

    void getAllCategories(
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
};