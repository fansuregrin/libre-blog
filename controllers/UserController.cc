#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>
#include "UserController.h"
#include "../utils/Utils.h"


void UserController::login(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const User &user
) const {
    orm::Mapper<User> mapper(app().getDbClient());

    mapper.findOne(
        {User::Cols::_username, user.getValueOfUsername()},
        [=] (const User &userInDb) {
            Json::Value json;
            auto passwdEnc = drogon::utils::getSha256(
                user.getValueOfPassword() + userInDb.getValueOfSalt());
            if (userInDb.getValueOfPassword() != passwdEnc) {
                json["status"] = 2;
            } else {
                json["status"] = 0;
                json_traits::integer_type uid{userInDb.getValueOfId()};
                auto token = jwt::create<json_traits>()
                    .set_type("JWT").set_issuer("drogon")
                    .set_issued_now().set_expires_in(std::chrono::seconds{3600})
                    .set_payload_claim("uid", uid)
                    .sign(jwt::algorithm::es256k(es256k_pub_key, es256k_priv_key));
                json["token"] = token;
            }
        
            auto resp = HttpResponse::newHttpJsonResponse(json);
            callback(resp);
        },
        [=] (const orm::DrogonDbException &ex) {
            Json::Value json;
            json["status"] = 2;
            auto resp = HttpResponse::newHttpJsonResponse(json);
            callback(resp);
        }
    );
}

void UserController::userCenter(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    Json::Value json;
    auto token = req->getHeader("Authorization").substr(7);
    auto decoded = jwt::decode<json_traits>(token);
    int userId = decoded.get_payload_claim("uid").as_integer();
    
    auto db = app().getDbClient();
    orm::Mapper<User> mp(db);
    try {
        auto userInDb = mp.findOne(Criteria(User::Cols::_id, userId));
        json["user"]["id"] = userInDb.getValueOfId();
        json["user"]["username"] = userInDb.getValueOfUsername();
        json["user"]["realname"] = userInDb.getValueOfRealname();
        json["user"]["email"] = userInDb.getValueOfEmail();
        json["user"]["role"] = userInDb.getRole(db).getValueOfName();
        json["user"]["create_time"] = userInDb.getValueOfCreateTime()
            .toDbStringLocal();
        json["status"] = 0;
    } catch (const orm::DrogonDbException &ex) {
        LOG_DEBUG << ex.base().what();
        json["status"] = 2;
    }
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void UserController::register_(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    User user
) const {
    Json::Value json;
    bool valid = true;

    // check password
    if (!checkPassword(user.getValueOfPassword())) {
        json["status"] = 4;
        json["error"] = "密码不合法";
        valid = false;
    }
    // check username
    else if (!checkUsername(user.getValueOfUsername())) {
        json["status"] = 5;
        json["error"] = "用户名不合法";
        valid = false;
    }
    // check email 
    else if (!checkEmail(user.getValueOfEmail())) {
        json["status"] = 6;
        json["error"] = "电子邮箱不合法";
        valid = false;
    }
    if (!valid) {
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    try {
        auto cnt = mpUser.count({User::Cols::_username, user.getValueOfUsername()});
        if (cnt > 0) {
            json["status"] = 7;
            json["error"] = "该用户名已经被注册";
        } else {
            auto uuid = utils::getUuid();
            user.setSalt(uuid);
            user.setPassword(
                utils::getSha256(user.getValueOfPassword() + uuid)
            );
            mpUser.insert(user);
            json["status"] = 0;
        }
    } catch (const orm::DrogonDbException &ex) {
        LOG_DEBUG << ex.base().what();
        json["status"] = 2;
    }
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void UserController::userList(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int page
) const {
    Json::Value json;
    auto token = req->getHeader("Authorization").substr(7);
    auto decoded = jwt::decode<json_traits>(token);
    int userId = decoded.get_payload_claim("uid").as_integer();

    if (page < 1) {
        json["status"] = 5;
        json["error"] = "页码不合法";
        callback(HttpResponse::newHttpJsonResponse(json));
    }

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    try {
        auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
        if (userInDb.getValueOfRole() <= 1) {
            // 只有 administrator(id=1) 才有获取用户列表的权限
            int perPage = 10;
            auto numUsers = mpUser.count();
            json["num_pages"] = numUsers / perPage + (numUsers%perPage?1:0);
            auto users = mpUser.paginate(page, perPage).findAll();
            for (const auto &user : users) {
                Json::Value userItem;
                userItem["id"] = user.getValueOfId();
                userItem["username"] = user.getValueOfUsername();
                userItem["realname"] = user.getValueOfRealname();
                userItem["email"] = user.getValueOfEmail();
                userItem["create_time"] = user.getValueOfCreateTime().toDbString();
                userItem["role"] = user.getRole(db).getValueOfName();
                json["users"].append(userItem);
            }
            json["status"] = 0;
        } else {
            json["status"] = 4;
            json["error"] = "没有权限";
        }
    } catch (const orm::DrogonDbException &ex) {
        LOG_DEBUG << ex.base().what();
        json["status"] = 2;
    }
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void UserController::updateGeneralInfo(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    auto token = req->getHeader("Authorization").substr(7);
    auto decoded = jwt::decode<json_traits>(token);
    int userId = decoded.get_payload_claim("uid").as_integer();

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
    Json::Value json;
    if (!checkUsername(username)) {
        json["status"] = 5;
        json["error"] = "用户名不合法";
    } else if (!checkEmail(email)) {
        json["status"] = 6;
        json["error"] = "电子邮件不合法";
    } else {
        Mapper<User> mp(app().getDbClient());
        try {    
            mp.updateBy(
                {User::Cols::_username, User::Cols::_email, User::Cols::_realname},
                {User::Cols::_id, userId},
                reqJson["username"], reqJson["email"], reqJson["realname"]
            );
            json["status"] = 0;
        } catch (const orm::DrogonDbException &ex) {
            LOG_DEBUG << ex.base().what();
            json["status"] = 2;
        }
    }
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void UserController::updatePassword(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    auto token = req->getHeader("Authorization").substr(7);
    auto decoded = jwt::decode<json_traits>(token);
    int userId = decoded.get_payload_claim("uid").as_integer();

    const auto &reqJson = *req->getJsonObject();
    if (!reqJson.isMember("password") || 
    reqJson["password"].type() != Json::stringValue) {
        throw std::invalid_argument("缺少必备字段: password, 或者类型错误");
    }
    auto password = reqJson["password"].asString();

    Json::Value json;
    Mapper<User> mp(app().getDbClient());
    if (checkPassword(password)) {
        try {
            auto userInDb = mp.findOne({User::Cols::_id, userId});
            mp.updateBy(
                {User::Cols::_password},
                {User::Cols::_id, userId},
                utils::getSha256(reqJson["password"].asString()+userInDb.getValueOfSalt())
            );
            json["status"] = 0;
        } catch (const orm::DrogonDbException &ex) {
            LOG_DEBUG << ex.base().what();
            json["status"] = 2;
        }
    } else {
        json["status"] = 4;
        json["error"] = "密码不合法";
    }
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}


void UserController::getUser(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int id
) const {
    Json::Value json;
    auto token = req->getHeader("Authorization").substr(7);
    auto decoded = jwt::decode<json_traits>(token);
    int userId = decoded.get_payload_claim("uid").as_integer();

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    try {
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
            json["user"] = user;
            json["status"] = 0;
        } else {
            json["status"] = 4;
            json["error"] = "没有权限";
        }
    } catch (const orm::DrogonDbException &ex) {
        LOG_DEBUG << ex.base().what();
        json["status"] = 2;
    }
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void UserController::addUser(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    User user
) const {
    Json::Value json;

    bool valid = true;
    // check password
    if (!checkPassword(user.getValueOfPassword())) {
        json["status"] = 5;
        json["error"] = "密码不合法";
        valid = false;
    }
    // check username
    else if (!checkUsername(user.getValueOfUsername())) {
        json["status"] = 6;
        json["error"] = "用户名不合法";
        valid = false;
    }
    // check email 
    else if (!checkEmail(user.getValueOfEmail())) {
        json["status"] = 7;
        json["error"] = "电子邮箱不合法";
        valid = false;
    }
    if (!valid) {
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }

    auto token = req->getHeader("Authorization").substr(7);
    auto decoded = jwt::decode<json_traits>(token);
    int userId = decoded.get_payload_claim("uid").as_integer();

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    try {
        auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
        if (userInDb.getValueOfRole() == 1) {
            // 只有 administrator(id=1) 才有添加用户的权限
            auto cnt = mpUser.count(
                {User::Cols::_username, user.getValueOfUsername()});
            if (cnt > 1) {
                json["status"] = 8;
                json["error"] = "用户名已被占用";
            } else {
                auto uuid = utils::getUuid();
                user.setSalt(uuid);
                user.setPassword(
                    utils::getSha256(user.getValueOfPassword() + uuid)
                );
                mpUser.insert(user);
                json["status"] = 0;
            }
        } else {
            json["status"] = 4;
            json["error"] = "没有权限";
        }
    } catch (const orm::DrogonDbException &ex) {
        LOG_DEBUG << ex.base().what();
        json["status"] = 2;
    }

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void UserController::updateUser(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    User user
) const {
    Json::Value json;

    bool valid = true;
    // check password
    if (!user.getValueOfPassword().empty() && 
    !checkPassword(user.getValueOfPassword())) {
        json["status"] = 5;
        json["errror"] = "密码不合法";
        valid = false;
    }
    // check username
    else if (!checkUsername(user.getValueOfUsername())) {
        json["status"] = 6;
        json["error"] = "用户名不合法";
        valid = false;
    }
    // check email 
    else if (!checkEmail(user.getValueOfEmail())) {
        json["status"] = 7;
        json["error"] = "电子邮箱不合法";
        valid = false;
    }
    if (!valid) {
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }
    if (!user.getRole()) {
        user.setRole(4);
    }

    auto token = req->getHeader("Authorization").substr(7);
    auto decoded = jwt::decode<json_traits>(token);
    int userId = decoded.get_payload_claim("uid").as_integer();

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    try {
        auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
        if (userInDb.getValueOfRole() == 1) {
            // 只有 administrator(id=1) 才有更新用户的权限
            auto oldUser = mpUser.findOne(
                {User::Cols::_username, user.getValueOfUsername()});
            if (oldUser.getValueOfId() != user.getValueOfId()) {
                json["status"] = 8;
                json["error"] = "用户名已被占用";
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
                json["status"] = 0;
            }
        } else {
            json["status"] = 4;
            json["error"] = "没有权限";
        }
    } catch (const orm::DrogonDbException &ex) {
        LOG_DEBUG << ex.base().what();
        json["status"] = 2;
    }

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void UserController::deleteUsers(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    Json::Value json;
    auto token = req->getHeader("Authorization").substr(7);
    auto decoded = jwt::decode<json_traits>(token);
    int userId = decoded.get_payload_claim("uid").as_integer();

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

    try {
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
            json["status"] = 0;
        } else {
            json["status"] = 4;
            json["error"] = "没有权限";
        }
    } catch (const orm::DrogonDbException &ex) {
        LOG_DEBUG << ex.base().what();
        json["status"] = 2;
    }

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void UserController::roleList(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    Json::Value json;
    auto token = req->getHeader("Authorization").substr(7);
    auto decoded = jwt::decode<json_traits>(token);
    int userId = decoded.get_payload_claim("uid").as_integer();

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    Mapper<Role> mpRole(db);
    try {
        auto loginedUser = mpUser.findOne(Criteria(User::Cols::_id, userId));
        if (loginedUser.getValueOfRole() == 1) {
            // 只有 administrator(id=1) 才能获取角色列表
            auto roles = mpRole.findAll();
            for (const auto &roleInDb : roles) {
                Json::Value role;
                role["id"] = roleInDb.getValueOfId();
                role["name"] = roleInDb.getValueOfName();
                json["roles"].append(role);
            }
            json["status"] = 0;
        } else {
            json["status"] = 4;
            json["error"] = "没有权限";
        }
    } catch (const orm::DrogonDbException &ex) {
        LOG_DEBUG << ex.base().what();
        json["status"] = 2;
    }

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void UserController::getRole(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    Json::Value json;
    auto token = req->getHeader("Authorization").substr(7);
    auto decoded = jwt::decode<json_traits>(token);
    int userId = decoded.get_payload_claim("uid").as_integer();

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    try {
        auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
        auto roleId = userInDb.getValueOfRole();
        auto roleName = userInDb.getRole(db).getValueOfName();
        Json::Value role;
        role["roleId"] = roleId;
        role["roleName"] = roleName;
        json["role"] = role;
        json["status"] = 0;
    } catch (const orm::DrogonDbException &ex) {
        LOG_DEBUG << ex.base().what();
        json["status"] = 2;
    }

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}