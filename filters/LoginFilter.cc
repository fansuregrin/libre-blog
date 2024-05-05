#include "LoginFilter.h"
#include "../utils/Utils.h"

using namespace drogon;

void LoginFilter::doFilter(const HttpRequestPtr &req,
FilterCallback &&fcb, FilterChainCallback &&fccb) {
    std::string token;
    auto tmp = req->getHeader("Authorization");
    if (!tmp.empty() && tmp.compare(0, 7, "Bearer ") == 0) {
        token = tmp.substr(7);
    }
    if (verifyUserToken(token)) {
        fccb();
        return;
    }
    
    Json::Value json;
    json["status"] = 3;
    json["error"] = "登录已失效";
    auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
    fcb(resp);
}
