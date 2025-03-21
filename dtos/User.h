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
    int role = -1;

    Json::Value toJson() const;
};

using UserPtr = std::shared_ptr<User>;