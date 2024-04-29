#pragma once

#include <drogon/HttpController.h>
#include <../models/User.h>

using namespace drogon;

class UserController : public drogon::HttpController<UserController>{
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(UserController::login, "/login", Post);
    ADD_METHOD_TO(UserController::userCenter, "/user/center", Get);
    ADD_METHOD_TO(UserController::addUser, "/user/add", Post);
    ADD_METHOD_TO(UserController::updateUser, "/user/update", Post);
    ADD_METHOD_TO(UserController::updatePassword, "/user/update/password", Post);
    METHOD_LIST_END

    void login(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const drogon_model::dg_test::User &user
    ) const;

    void userCenter(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void addUser(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        const drogon_model::dg_test::User &user
    ) const;

    void updateUser(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void updatePassword(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;
};

namespace drogon {
template <>
inline drogon_model::dg_test::User fromRequest(const HttpRequest &req) {
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
    drogon_model::dg_test::User user;
    user.updateByJson(json);
    return user;
}
}

bool checkEmail(const std::string &email);
bool checkUsername(const std::string &username);
bool checkPassword(const std::string &password);
bool verifyUserToken(const std::string &token, int &userId);