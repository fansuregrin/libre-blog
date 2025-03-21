#include "User.h"

Json::Value User::toJson() const {
    Json::Value r;
    r["id"] = id;
    r["username"] = username;
    r["email"] = email;
    r["realname"] = realname;
    r["createTime"] = createTime;
    r["modifyTime"] = modifyTime;
    return r;
}