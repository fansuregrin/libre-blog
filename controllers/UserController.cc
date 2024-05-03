#include <regex>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>
#include "UserController.h"
#include "Utils.h"

using namespace orm;
using namespace drogon_model::dg_test;
using json_traits = jwt::traits::nlohmann_json;


const auto emailPattern = std::regex(
    R"(^[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)*@(\w+)(\.(\w+))+$)"
);


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
        [=] (const DrogonDbException &ex) {
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
    auto tmp = req->getHeader("Authorization");
    if (tmp.empty() || tmp.compare(0, 7, "Bearer ") != 0) {
        Json::Value json;
        json["status"] = 2;
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }
    auto token = tmp.substr(7);
    int userId = -1;
    if (!verifyUserToken(token, userId)) {
        Json::Value json;
        json["status"] = 3;
        json["error"] = "登录已失效";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }
    orm::Mapper<User> mapper(app().getDbClient());
    mapper.findOne(
        {User::Cols::_id, userId},
        [=] (const User &userInDb) {
            Json::Value json;
            json["status"] = 0;
            json["user"]["id"] = userInDb.getValueOfId();
            json["user"]["username"] = userInDb.getValueOfUsername();
            json["user"]["realname"] = userInDb.getValueOfRealname();
            json["user"]["email"] = userInDb.getValueOfEmail();
            json["user"]["create_time"] = userInDb.getValueOfCreateTime()
                .toDbStringLocal();
            auto resp = HttpResponse::newHttpJsonResponse(json);
            callback(resp);
        },
        [=] (const DrogonDbException &ex) {
            LOG_DEBUG << ex.base().what();
            Json::Value json;
            json["status"] = 2;
            auto resp = HttpResponse::newHttpJsonResponse(json);
            callback(resp);
        }
    );
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
                    [=] (const DrogonDbException &ex) {
                        Json::Value json;
                        json["status"] = 2;
                        json["error"] = "注册失败";
                        auto resp = HttpResponse::newHttpJsonResponse(json);
                        callback(resp);
                    }
                );
            }
        },
        [=] (const DrogonDbException &ex) {
            Json::Value json;
            json["status"] = 2;
            json["error"] = "注册失败";
            auto resp = HttpResponse::newHttpJsonResponse(json);
            callback(resp);
        }
    );
}

void UserController::updateUser(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    auto tmp = req->getHeader("Authorization");
    if (tmp.empty() || tmp.compare(0, 7, "Bearer ") != 0) {
        Json::Value json;
        json["status"] = 2;
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }
    auto token = tmp.substr(7);
    int userId = -1;
    if (!verifyUserToken(token, userId)) {
        Json::Value json;
        json["status"] = 3;
        json["error"] = "登录已失效";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }

    auto pJson = req->getJsonObject();
    if (pJson == nullptr) {
       throw std::invalid_argument("请求体格式错误, 请使用json"); 
    }
    const auto &reqJson = *pJson;
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
                {User::Cols::_username, User::Cols::_email},
                {User::Cols::_id, userId},
                reqJson["username"], reqJson["email"]
            );
            json["status"] = 0;
        } catch (const DrogonDbException &ex) {
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
    auto tmp = req->getHeader("Authorization");
    if (tmp.empty() || tmp.compare(0, 7, "Bearer ") != 0) {
        Json::Value json;
        json["status"] = 2;
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }
    auto token = tmp.substr(7);
    int userId = -1;
    if (!verifyUserToken(token, userId)) {
        Json::Value json;
        json["status"] = 3;
        json["error"] = "登录已失效";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }

    auto pJson = req->getJsonObject();
    if (pJson == nullptr) {
       throw std::invalid_argument("请求体格式错误, 请使用json"); 
    }
    const auto &reqJson = *pJson;
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
        } catch (const DrogonDbException &ex) {
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

bool checkEmail(const std::string &email) {
    if (email.empty()) return false;
    std::smatch res;
    return std::regex_match(email, res, emailPattern);
}

bool checkUsername(const std::string &username) {
    if (username.size() < 1) return false;
    if (std::isspace(*username.cbegin()) || std::isspace(*username.crbegin())) {
        return false;
    }
    return true;
}

bool checkPassword(const std::string &password) {
    if (password.size() < 6) return false;
    for (const auto &ch:password) {
        if (std::isspace(ch)) {
            return false;
        }
    } 
    return true;
}