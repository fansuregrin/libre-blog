#include "LoginFilter.h"
#include "../utils/Utils.h"
#include "../dtos/ApiResponse.h"

using namespace drogon;

void LoginFilter::doFilter(const HttpRequestPtr &req,
FilterCallback &&fcb, FilterChainCallback &&fccb) {
    std::string token;
    auto tmp = req->getHeader("Authorization");
    if (!tmp.empty() && tmp.compare(0, 7, "Bearer ") == 0) {
        token = tmp.substr(7);
    }
    
    try {
        auto decoded = jwt::decode<json_traits>(token);
        jwt::verify<json_traits>()
            .allow_algorithm(jwt::algorithm::es256k(es256k_pub_key, es256k_priv_key))
            .with_issuer("drogon")
            .verify(decoded);
        auto payload = decoded.get_payload_json();
        req->getAttributes()->insert("uid", payload["uid"].asInt());
        fccb();
        return;
    } catch (const std::exception &ex) {
        LOG_DEBUG << ex.what();
        auto respBody = ApiResponse::error(1, "登录已失效");
        auto resp = HttpResponse::newHttpJsonResponse(respBody.toJson());
        resp->setStatusCode(k401Unauthorized);
        fcb(resp);
        return;
    }
}
