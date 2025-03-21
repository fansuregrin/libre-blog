#include "UserController.h"

void UserController::login(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const User &user
) const {
    auto userInDb = UserMapper::selectByUsername(user.username);
    if (verfiyPassword(user.password, userInDb->password)) {
        json_traits::integer_type uid{userInDb->id};
        Json::Value data = jwt::create<json_traits>()
            .set_type("JWT").set_issuer("drogon")
            .set_issued_now().set_expires_in(std::chrono::seconds{3600})
            .set_payload_claim("uid", uid)
            .sign(jwt::algorithm::es256k(es256k_pub_key, es256k_priv_key));
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success(data).toJson());
        callback(resp);
    } else {
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::error(1, "用户名或密码错误").toJson());
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
        callback(resp);
    }
}

void UserController::userCenter(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int userId = req->getAttributes()->get<int>("uid");
    auto user = UserMapper::select(userId);
    Json::Value data = user ? user->toJson() : Json::nullValue;
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void UserController::register_(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    User user
) const {
    ApiResponse respBody;
    
    bool valid = true;
    // check username
    if (!checkUsername(user.username)) {
        respBody.setCodeAndMsg(1, "用户名不合法");
        valid = false;
    }
    // check password
    else if (!checkPassword(user.password)) {
        respBody.setCodeAndMsg(1, "密码不合法");
        valid = false;
    }
    // check email 
    else if (!checkEmail(user.email)) {
        respBody.setCodeAndMsg(1, "电子邮箱不合法");
        valid = false;
    }
    if (!valid) {
        auto resp = HttpResponse::newHttpJsonResponse(respBody.toJson());
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
        callback(resp);
        return;
    }

    auto cnt = UserMapper::countByUsername(user.username);
    if (cnt > 0) {
        respBody.setCodeAndMsg(1, "该用户名已经被注册");
        auto resp = HttpResponse::newHttpJsonResponse(
            respBody.toJson()
        );
        resp->setStatusCode(HttpStatusCode::k409Conflict);
        callback(resp);
    } else {
        user.password = encodePassword(user.password);
        user.role = Role::SUBSCRIBER;
        UserMapper::insert(user);
        respBody.setCodeAndMsg(0, "success");
        auto resp = HttpResponse::newHttpJsonResponse(
            respBody.toJson()
        );
        callback(resp);
    }
}

void UserController::userList(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int userId = req->getAttributes()->get<int>("uid");
    Json::Value data;
    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");
    int roleId = UserMapper::selectRoleId(userId);
    if (roleId <= Role::ADMINISTRATOR) {
        // 只有 administrator(id=1) 才有获取用户列表的权限
        data["total"] = UserMapper::count();
        auto users = UserMapper::selectLimit(page, pageSize);
        data["users"] = toJson(users);
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success(data).toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}

void UserController::updateGeneralInfo(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    User user
) const {
    if (!checkUsername(user.username)) {
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::error(1, "用户名不合法").toJson()
        );
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
        callback(resp);
    } else if (!checkEmail(user.email)) {
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::error(1, "电子邮件不合法").toJson()
        );
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
        callback(resp);
    } else {
        user.id = req->getAttributes()->get<int>("uid");
        auto oldUser = UserMapper::select(user.id);
        if (oldUser->id != user.id) {
            throw std::runtime_error("用户名被占用");
        }
        user.password.clear();
        user.role = -1;
        UserMapper::update(user);
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    }
}

void UserController::updatePassword(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    Password password
) const {
    int userId = req->getAttributes()->get<int>("uid");
    const std::string &oldPassword = password.oldPassword;
    const std::string &newPassword = password.newPassword;
    
    if (checkPassword(oldPassword) && checkPassword(newPassword)) {
        auto userInDb = UserMapper::select(userId);
        std::string oldPasswdEnc = encodePassword(oldPassword);
        if (oldPasswdEnc == userInDb->password) {
            User user;
            user.password = encodePassword(newPassword);
            UserMapper::update(user);
            auto resp = HttpResponse::newHttpJsonResponse(
                ApiResponse::success().toJson()
            );
            callback(resp);
        } else {
            auto resp = HttpResponse::newHttpJsonResponse(
                ApiResponse::error(1, "旧密码错误").toJson()
            );
            resp->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(resp);
        }
    } else {
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::error(1, "密码不合法").toJson()
        );
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
        callback(resp);
    }
}

void UserController::getUser(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int id
) const {
    auto user = UserMapper::select(id);
    Json::Value data = user ? user->toJson() : Json::nullValue;
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void UserController::addUser(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    User user
) const {
    ApiResponse respBody;

    bool valid = true;
    // check password
    if (!checkPassword(user.password)) {
        respBody.setCodeAndMsg(1, "密码不合法");
        valid = false;
    }
    // check username
    else if (!checkUsername(user.username)) {
        respBody.setCodeAndMsg(1, "用户名不合法");
        valid = false;
    }
    // check email 
    else if (!checkEmail(user.email)) {
        respBody.setCodeAndMsg(1, "电子邮箱不合法");
        valid = false;
    }
    if (!valid) {
        auto resp = HttpResponse::newHttpJsonResponse(
            respBody.toJson()
        );
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
        callback(resp);
        return;
    }

    int userId = req->getAttributes()->get<int>("uid");
    int roleId = UserMapper::selectRoleId(userId);
    // 只有 administrator(id=1) 才有添加用户的权限
    if (roleId == Role::ADMINISTRATOR) {
        auto cnt = UserMapper::countByUsername(user.username);
        if (cnt > 0) {
            respBody.setCodeAndMsg(1, "用户名已被占用");
            auto resp = HttpResponse::newHttpJsonResponse(
                respBody.toJson()
            );
            resp->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(resp);
        } else {
            user.password = encodePassword(user.password);
            UserMapper::insert(user);
            auto resp = HttpResponse::newHttpJsonResponse(
                ApiResponse::success().toJson()
            );
            callback(resp);
        }
    } else {
        throw PermissionException();
    }
}

void UserController::updateUser(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    User user
) const {
    Json::Value json;

    ApiResponse respBody;
    bool valid = true;
    // check username
    if (!checkUsername(user.username)) {
        respBody.setCodeAndMsg(1, "用户名不合法");
        valid = false;
    }
    // check email 
    else if (!checkEmail(user.email)) {
        respBody.setCodeAndMsg(1, "电子邮箱不合法");
        valid = false;
    }
    if (!valid) {
        auto resp = HttpResponse::newHttpJsonResponse(
            respBody.toJson()
        );
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
        callback(resp);
        return;
    }

    int userId = req->getAttributes()->get<int>("uid");
    int roleId = UserMapper::selectRoleId(userId);
    // 只有 administrator(id=1) 才有更新用户的权限
    if (roleId == Role::ADMINISTRATOR) {
        auto oldUser = UserMapper::selectByUsername(user.username);
        if (oldUser->id != user.id) {
            respBody.setCodeAndMsg(1, "用户名已被占用");
            auto resp = HttpResponse::newHttpJsonResponse(
                respBody.toJson()
            );
            resp->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(resp);
        } else {
            UserMapper::update(user);
            auto resp = HttpResponse::newHttpJsonResponse(
                ApiResponse::success().toJson()
            );
            callback(resp);
        }
    } else {
        throw PermissionException();
    }
}

void UserController::deleteUsers(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int userId = req->getAttributes()->get<int>("uid");

    const auto &reqJson = *req->getJsonObject();
    if (!reqJson.isMember("ids") || reqJson["ids"].type() != Json::arrayValue) {
        throw std::invalid_argument("缺少必备字段: ids, 或者类型错误");
    }

    std::vector<int> ids;
    for (const auto &id : reqJson["ids"]) {
        if (id.asInt() > 1) {
            ids.emplace_back(id.asInt());
        }
    }

    int roleId = UserMapper::selectRoleId(userId);
    // 只有 administrator(id=1) 才有删除用户的权限
    if (roleId == Role::ADMINISTRATOR) {
        if (!ids.empty()) {
            UserMapper::deletes(ids);
            // todo: 删除该 user 撰写的文章以及文章与标签的关系
            ArticleMapper::deleteByUsers(ids);
            ArticleTagMapper::deleteByUsers(ids);
        }
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}
