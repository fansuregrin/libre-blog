#pragma once

#include <string>
#include <json/json.h>

struct ApiResponse {
    int code;
    std::string msg;
    Json::Value data;

    Json::Value toJson() const;
    void setCodeAndMsg(int code, const std::string &msg);
    static ApiResponse success(const Json::Value &data = Json::Value());
    static ApiResponse error(int code, const std::string &msg);
};