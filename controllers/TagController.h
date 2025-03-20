#pragma once

#include <drogon/drogon.h>
#include "../utils/Utils.h"
#include "../dtos/Tag.h"
#include "../dtos/ApiResponse.h"
#include "../dtos/Role.h"
#include "../mappers/TagMapper.h"
#include "../mappers/UserMapper.h"
#include "../exceptions/PermissionException.h"

using namespace drogon;

class TagController : public HttpController<TagController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(TagController::getAllTags, "/tags/all", HttpMethod::Get);
    ADD_METHOD_TO(TagController::tagList, "/tags", HttpMethod::Get, "PaginationFilter");
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
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void getTag(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int id
    ) const;

    void addTag(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        Tag tag
    ) const;

    void updateTag(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        Tag tag
    ) const;

    void deleteTags(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;
};