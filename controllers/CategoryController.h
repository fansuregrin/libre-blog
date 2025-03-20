#pragma once

#include <drogon/drogon.h>
#include "TransformRequest.h"
#include "../utils/Utils.h"
#include "../dtos/Category.h"
#include "../dtos/ApiResponse.h"
#include "../mappers/CategoryMapper.h"
#include "../exceptions/PermissionException.h"

using namespace drogon;

class CategoryController : public HttpController<CategoryController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(CategoryController::getAllCategories, "/categories", HttpMethod::Get);
    ADD_METHOD_TO(CategoryController::getCategory, "/category/{id}", HttpMethod::Get);
    ADD_METHOD_TO(CategoryController::getCategoryBySlug, "/category/{slug}", HttpMethod::Get); 
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

    void getCategoryBySlug(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const std::string &slug
    ) const;

    void addCategory(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        Category category
    ) const;

    void updateCategory(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        Category category
    ) const;

    void deleteCategories(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;
};