#include "JsonFilter.h"

using namespace drogon;

void JsonFilter::doFilter(
    const HttpRequestPtr &req,
    FilterCallback &&fcb,
    FilterChainCallback &&fccb
) {
    if (req->getJsonObject() != nullptr) {
        fccb();
        return;
    }
    Json::Value json;
    json["status"] = 2;
    json["error"] = "请求体格式错误, 请使用json";
    auto resp = HttpResponse::newHttpJsonResponse(json);
    fcb(resp);
}