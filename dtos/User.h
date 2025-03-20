#pragma once

#include <string>
#include <json/json.h>

struct User {
    int id = -1;
    std::string username;
    std::string password;
    std::string createTime;
    std::string modifyTime;
    std::string email;
    std::string realname;
    std::string salt;
    int role = -1;

    Json::Value toJson() const {
        Json::Value r;
        r["id"] = id;
        r["username"] = username;
        r["email"] = email;
        r["realname"] = realname;
        r["createTime"] = createTime;
        r["modifyTime"] = modifyTime;
        return r;
    }
};

using UserPtr = std::shared_ptr<User>;