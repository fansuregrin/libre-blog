#include "UserController.h"

void UserController::login(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const User &user
) const {
    orm::Mapper<User> mapper(app().getDbClient());

    mapper.findOne(
        {User::Cols::_username, user.getValueOfUsername()},
        [=] (const User &userInDb) {
            Json::Value data;
            auto passwdEnc = drogon::utils::getSha256(
                user.getValueOfPassword() + userInDb.getValueOfSalt());
            if (userInDb.getValueOfPassword() != passwdEnc) {
                auto resp = HttpResponse::newHttpJsonResponse(
                    ApiResponse::error(1, "登录失败").toJson());
                resp->setStatusCode(HttpStatusCode::k400BadRequest);
                callback(resp);
            } else {
                json_traits::integer_type uid{userInDb.getValueOfId()};
                data = jwt::create<json_traits>()
                    .set_type("JWT").set_issuer("drogon")
                    .set_issued_now().set_expires_in(std::chrono::seconds{3600})
                    .set_payload_claim("uid", uid)
                    .sign(jwt::algorithm::es256k(es256k_pub_key, es256k_priv_key));
                auto resp = HttpResponse::newHttpJsonResponse(
                    ApiResponse::success(data).toJson());
                callback(resp);
            }
        },
        [=] (const orm::DrogonDbException &ex) {
            auto resp = HttpResponse::newHttpJsonResponse(
                ApiResponse::error(1, "登录失败").toJson()
            );
            resp->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(resp);
        }
    );
}

void UserController::userCenter(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    auto db = app().getDbClient();
    orm::Mapper<User> mp(db);
    int userId = req->getAttributes()->get<int>("uid");
    auto userInDb = mp.findOne(Criteria(User::Cols::_id, userId));
    Json::Value data;
    data["id"] = userInDb.getValueOfId();
    data["username"] = userInDb.getValueOfUsername();
    data["realname"] = userInDb.getValueOfRealname();
    data["email"] = userInDb.getValueOfEmail();
    data["role"] = userInDb.getRole(db).getValueOfName();
    data["create_time"] = userInDb.getValueOfCreateTime()
        .toDbStringLocal();
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

    // check password
    if (!checkPassword(user.getValueOfPassword())) {
        respBody.setCodeAndMsg(1, "密码不合法");
        valid = false;
    }
    // check username
    else if (!checkUsername(user.getValueOfUsername())) {
        respBody.setCodeAndMsg(1, "用户名不合法");
        valid = false;
    }
    // check email 
    else if (!checkEmail(user.getValueOfEmail())) {
        respBody.setCodeAndMsg(1, "电子邮箱不合法");
        valid = false;
    }
    if (!valid) {
        auto resp = HttpResponse::newHttpJsonResponse(respBody.toJson());
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
        callback(resp);
        return;
    }

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    auto cnt = mpUser.count({User::Cols::_username, user.getValueOfUsername()});
    if (cnt > 0) {
        respBody.setCodeAndMsg(1, "该用户名已经被注册");
        auto resp = HttpResponse::newHttpJsonResponse(
            respBody.toJson()
        );
        resp->setStatusCode(HttpStatusCode::k409Conflict);
        callback(resp);
    } else {
        auto uuid = utils::getUuid();
        user.setSalt(uuid);
        user.setPassword(
            utils::getSha256(user.getValueOfPassword() + uuid)
        );
        mpUser.insert(user);
        respBody.setCodeAndMsg(0, "success");
        auto resp = HttpResponse::newHttpJsonResponse(
            respBody.toJson()
        );
        callback(resp);
    }
}

void UserController::userList(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int page
) const {
    if (page < 1) {
        throw PageException();
    }
    
    int userId = req->getAttributes()->get<int>("uid");
    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    Json::Value data;
    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    if (userInDb.getValueOfRole() <= 1) {
        // 只有 administrator(id=1) 才有获取用户列表的权限
        int perPage = 10;
        auto numUsers = mpUser.count();
        data["num_pages"] = numUsers / perPage + (numUsers%perPage?1:0);
        auto users = mpUser.paginate(page, perPage).findAll();
        for (const auto &user : users) {
            Json::Value userItem;
            userItem["id"] = user.getValueOfId();
            userItem["username"] = user.getValueOfUsername();
            userItem["realname"] = user.getValueOfRealname();
            userItem["email"] = user.getValueOfEmail();
            userItem["create_time"] = user.getValueOfCreateTime().toDbString();
            userItem["role"] = user.getRole(db).getValueOfName();
            data["users"].append(userItem);
        }
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
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int userId = req->getAttributes()->get<int>("uid");

    const auto &reqJson = *req->getJsonObject();
    if (!reqJson.isMember("username") || 
    reqJson["username"].type() != Json::stringValue) {
        throw std::invalid_argument("缺少必备字段: username, 或者类型错误");
    }
    if (!reqJson.isMember("email") || 
    reqJson["email"].type() != Json::stringValue) {
        throw std::invalid_argument("缺少必备字段: email, 或者类型错误");
    }

    auto username = reqJson["username"].asString();
    auto email = reqJson["email"].asString();
    if (!checkUsername(username)) {
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::error(1, "用户名不合法").toJson()
        );
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
        callback(resp);
    } else if (!checkEmail(email)) {
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::error(1, "电子邮件不合法").toJson()
        );
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
        callback(resp);
    } else {
        Mapper<User> mp(app().getDbClient());
        mp.updateBy(
            {User::Cols::_username, User::Cols::_email, User::Cols::_realname},
            {User::Cols::_id, userId},
            reqJson["username"], reqJson["email"], reqJson["realname"]
        );
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
    Mapper<User> mp(app().getDbClient());
    if (checkPassword(oldPassword) && checkPassword(newPassword)) {
        auto userInDb = mp.findOne({User::Cols::_id, userId});
        std::string oldPasswdEnc = utils::getSha256(oldPassword + userInDb.getValueOfSalt());
        if (oldPasswdEnc == userInDb.getValueOfPassword()) {
            std::string newSalt = utils::getUuid();
            std::string newPasswdEnc = utils::getSha256(newPassword + newSalt);
            mp.updateBy(
                {User::Cols::_password, User::Cols::_salt},
                {User::Cols::_id, userId},
                newPasswdEnc, newSalt
            );
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
    int userId = req->getAttributes()->get<int>("uid");
    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    auto loginedUser = mpUser.findOne(Criteria(User::Cols::_id, userId));
    if (loginedUser.getValueOfRole() == 1) {
        auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, id));
        Json::Value user;
        user["id"] = id;
        user["username"] = userInDb.getValueOfUsername();
        user["realname"] = userInDb.getValueOfRealname();
        user["email"] = userInDb.getValueOfEmail();
        auto role = userInDb.getRole(db);
        user["role"]["id"] = role.getValueOfId();
        user["role"]["name"] = role.getValueOfName();
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success(user).toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}

void UserController::addUser(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    User user
) const {
    ApiResponse respBody;

    bool valid = true;
    // check password
    if (!checkPassword(user.getValueOfPassword())) {
        respBody.setCodeAndMsg(1, "密码不合法");
        valid = false;
    }
    // check username
    else if (!checkUsername(user.getValueOfUsername())) {
        respBody.setCodeAndMsg(1, "用户名不合法");
        valid = false;
    }
    // check email 
    else if (!checkEmail(user.getValueOfEmail())) {
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
    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    if (userInDb.getValueOfRole() == 1) {
        // 只有 administrator(id=1) 才有添加用户的权限
        auto cnt = mpUser.count(
            {User::Cols::_username, user.getValueOfUsername()});
        if (cnt > 1) {
            respBody.setCodeAndMsg(1, "用户名已被占用");
            auto resp = HttpResponse::newHttpJsonResponse(
                respBody.toJson()
            );
            resp->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(resp);
        } else {
            auto uuid = utils::getUuid();
            user.setSalt(uuid);
            user.setPassword(
                utils::getSha256(user.getValueOfPassword() + uuid)
            );
            mpUser.insert(user);
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
    // check password
    if (!user.getValueOfPassword().empty() && 
    !checkPassword(user.getValueOfPassword())) {
        respBody.setCodeAndMsg(1, "密码不合法");
        valid = false;
    }
    // check username
    else if (!checkUsername(user.getValueOfUsername())) {
        respBody.setCodeAndMsg(1, "用户名不合法");
        valid = false;
    }
    // check email 
    else if (!checkEmail(user.getValueOfEmail())) {
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
    if (!user.getRole()) {
        user.setRole(4);
    }

    int userId = req->getAttributes()->get<int>("uid");

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    if (userInDb.getValueOfRole() == 1) {
        // 只有 administrator(id=1) 才有更新用户的权限
        auto oldUser = mpUser.findOne(
            {User::Cols::_username, user.getValueOfUsername()});
        if (oldUser.getValueOfId() != user.getValueOfId()) {
            respBody.setCodeAndMsg(1, "用户名已被占用");
            auto resp = HttpResponse::newHttpJsonResponse(
                respBody.toJson()
            );
            resp->setStatusCode(HttpStatusCode::k400BadRequest);
            callback(resp);
        } else {
            if (!user.getValueOfPassword().empty()) {
                auto uuid = utils::getUuid();
                user.setSalt(uuid);
                user.setPassword(
                    utils::getSha256(user.getValueOfPassword() + uuid)
                );
                mpUser.update(user);
            } else {
                mpUser.updateBy(
                    {User::Cols::_username, User::Cols::_realname, 
                    User::Cols::_email, User::Cols::_role},
                    {User::Cols::_id, user.getValueOfId()},
                    user.getValueOfUsername(), user.getValueOfRealname(),
                    user.getValueOfEmail(), user.getValueOfRole()
                );
            }
            mpUser.insert(user);
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

    std::vector<int> userIds;
    for (const auto &id : reqJson["ids"]) {
        if (id.asInt() > 1) {
            userIds.emplace_back(id.asInt());
        }
    }

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    Mapper<ArticleTag> mpArticleTag(db);
    Mapper<Article> mpArticle(db);

    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    if (userInDb.getValueOfRole() == 1) {
        // 只有 administrator(id=1) 才有删除用户的权限
        if (!userIds.empty()) {
            // 在删除用户之前需要删除用户撰写的所有文章
            auto articles = mpArticle.findBy(
                Criteria(Article::Cols::_author, CompareOperator::In, userIds));
            std::vector<int> artIds;
            for (const auto &art : articles) {
                artIds.emplace_back(art.getValueOfId());
            }
            if (!artIds.empty()) {
                mpArticleTag.deleteBy(
                    Criteria(ArticleTag::Cols::_article, CompareOperator::In, artIds));
                mpArticle.deleteBy(
                    Criteria(Article::Cols::_id, CompareOperator::In, artIds));
            }
            mpUser.deleteBy(
                Criteria(User::Cols::_id, CompareOperator::In, userIds));
        }
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}

void UserController::getRole(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int userId = req->getAttributes()->get<int>("uid");
    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    auto roleId = userInDb.getValueOfRole();
    auto roleName = userInDb.getRole(db).getValueOfName();
    Json::Value role;
    role["roleId"] = roleId;
    role["roleName"] = roleName;
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(role).toJson()
    );
    callback(resp);
}