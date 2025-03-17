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
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::error(1, "请求体格式错误, 请使用json").toJson()
    );
    resp->setStatusCode(HttpStatusCode::k400BadRequest);
    fcb(resp);
}