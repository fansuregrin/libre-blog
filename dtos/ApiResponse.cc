#include "ApiResponse.h"

Json::Value ApiResponse::toJson() const {
    Json::Value result;
    result["code"] = code;
    result["msg"] = msg;
    result["data"] = data;
    return result;
}

void ApiResponse::setCodeAndMsg(int code, const std::string &msg) {
    this->code = code;
    this->msg = msg;
}

ApiResponse ApiResponse::success(const Json::Value &data) {
    return {0, "success", data};
}

ApiResponse ApiResponse::error(int code, const std::string &msg) {
    return {code, msg, Json::Value()};
}