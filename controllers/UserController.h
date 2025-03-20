#pragma once

#include <drogon/HttpController.h>
#include "TransformRequest.h"
#include "../utils/Utils.h"
#include "../dtos/User.h"
#include "../dtos/Role.h"
#include "../dtos/ApiResponse.h"
#include "../mappers/UserMapper.h"
#include "../exceptions/PermissionException.h"

using namespace drogon;

class UserController : public HttpController<UserController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserController::login, "/login", HttpMethod::Post);
    ADD_METHOD_TO(UserController::register_, "/register", HttpMethod::Post, "JsonFilter");
    ADD_METHOD_TO(UserController::userCenter, "/admin/user/center", HttpMethod::Get, "LoginFilter");
    ADD_METHOD_TO(UserController::userList, "/admin/users", HttpMethod::Get, "LoginFilter", "PaginationFilter");
    ADD_METHOD_TO(UserController::updateGeneralInfo, "/admin/user/general-info", HttpMethod::Patch, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(UserController::updatePassword, "/admin/user/password", HttpMethod::Patch, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(UserController::getUser, "/user/{id}", HttpMethod::Get);
    ADD_METHOD_TO(UserController::addUser, "/admin/user", HttpMethod::Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(UserController::updateUser, "/admin/user", HttpMethod::Put, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(UserController::deleteUsers, "/admin/user", HttpMethod::Delete, "LoginFilter", "JsonFilter");
    METHOD_LIST_END

    void login(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const User &user
    ) const;

    void userCenter(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void register_(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        User user
    ) const;

    void getUser(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        int id
    ) const;

    void addUser(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        User user
    ) const;

    void userList(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void updateGeneralInfo(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        User user
    ) const;

    void updatePassword(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        Password password
    ) const;

    void updateUser(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        User user
    ) const;

    void deleteUsers(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;
};
