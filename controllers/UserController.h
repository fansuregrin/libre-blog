#pragma once

#include <drogon/HttpController.h>
#include <../models/User.h>
#include <../models/Role.h>
#include <../models/Article.h>
#include <../models/ArticleTag.h>

using namespace drogon;
using drogon_model::libre_blog::User;
using drogon_model::libre_blog::Role;
using drogon_model::libre_blog::Article;
using drogon_model::libre_blog::ArticleTag;
using orm::Mapper;
using orm::Criteria;
using orm::CompareOperator;
using json_traits = jwt::traits::nlohmann_json;


class UserController : public HttpController<UserController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserController::login, "/login", Post);
    ADD_METHOD_TO(UserController::userCenter, "/user/center", Get, "LoginFilter");
    ADD_METHOD_TO(UserController::register_, "/user/register", Post, "JsonFilter");
    ADD_METHOD_TO(UserController::userList, "/users/{page}", Get, "LoginFilter");
    ADD_METHOD_TO(UserController::updateGeneralInfo, "/user/update/general-info", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(UserController::updatePassword, "/user/update/password", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(UserController::getUser, "/user/{id}", Get, "LoginFilter");
    ADD_METHOD_TO(UserController::addUser, "/user/add", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(UserController::updateUser, "/user/update", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(UserController::deleteUsers, "/user/delete", Post, "LoginFilter", "JsonFilter");
    ADD_METHOD_TO(UserController::roleList, "/roles", Get, "LoginFilter");
    ADD_METHOD_TO(UserController::getRole, "/user/role", Get, "LoginFilter");
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
        std::function<void (const HttpResponsePtr &)> &&callback,
        int page
    ) const;

    void updateGeneralInfo(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void updatePassword(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
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

    void roleList(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void getRole(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;
};

namespace drogon {
template <>
inline User fromRequest(const HttpRequest &req) {
    auto jsonPtr = req.getJsonObject();
    if (jsonPtr == nullptr) {
        throw std::invalid_argument("请求体格式错误, 请使用json");
    }
    auto &json = *jsonPtr;
    if (!json.isMember("username") || 
    json["username"].type() != Json::stringValue) {
        throw std::invalid_argument("缺少必备字段: username, 或者类型错误");
    }
    if (!json.isMember("password") || 
    json["password"].type() != Json::stringValue) {
        throw std::invalid_argument("缺少必备字段: password, 或者类型错误");
    }
    User user;
    user.updateByJson(json);
    if (user.getValueOfRealname().empty()) {
        user.setRealname(user.getValueOfUsername());
    }
    return user;
}
}