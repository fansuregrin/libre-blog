#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>
#include "UserController.h"
#include "../utils/Utils.h"


void UserController::login(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const drogon_model::dg_test::User &user
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

void UserController::addUser(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const drogon_model::dg_test::User &user
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

    Mapper<User> mapper(app().getDbClient());
    mapper.findBy(
        {User::Cols::_username, user.getValueOfUsername()},
        [=] (const std::vector<User> &usersInDb) {
            Json::Value json;
            if (usersInDb.size() > 0) {
                json["status"] = 7;
                json["error"] = "该用户名已经被注册";
                auto resp = HttpResponse::newHttpJsonResponse(json);
                callback(resp);
                return;
            } else {
                User user_ = user;
                auto uuid = utils::getUuid();
                user_.setSalt(uuid);
                user_.setPassword(
                    utils::getSha256(user_.getValueOfPassword() + uuid)
                );
                Mapper<User> m(app().getDbClient());
                m.insert(
                    user_,
                    [=] (const User &u) {
                        Json::Value json;
                        json["status"] = 0;
                        json["message"] = "注册成功";
                        auto resp = HttpResponse::newHttpJsonResponse(json);
                        callback(resp);
                    },
                    [=] (const orm::DrogonDbException &ex) {
                        Json::Value json;
                        json["status"] = 2;
                        json["error"] = "注册失败";
                        auto resp = HttpResponse::newHttpJsonResponse(json);
                        callback(resp);
                    }
                );
            }
        },
        [=] (const orm::DrogonDbException &ex) {
            Json::Value json;
            json["status"] = 2;
            json["error"] = "注册失败";
            auto resp = HttpResponse::newHttpJsonResponse(json);
            callback(resp);
        }
    );
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

void UserController::updateUser(
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