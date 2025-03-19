#include "PaginationFilter.h"

void PaginationFilter::doFilter(
    const HttpRequestPtr &req, FilterCallback &&fcb, 
    FilterChainCallback &&fccb
) {
    ApiResponse respBody;
    int page = 1, pageSize = 5;
    
    auto pageStr = req->getParameter("page");
    auto pageSizeStr = req->getParameter("pageSize");
    bool canTrans = true;
    if (!pageStr.empty()) {
        try {
            page = std::stoi(pageStr);
        } catch (std::exception &ex) {
            canTrans = false;
        }
    }
    if (!pageSizeStr.empty()) {
        try {
            pageSize = std::stoi(pageSizeStr);
        } catch (std::exception &ex) {
            canTrans = false;
        }
    }
    
    if (!canTrans || page < 1 || pageSize < 1) {
        respBody.setCodeAndMsg(1, "无效的页码或页大小");
        auto resp = HttpResponse::newHttpJsonResponse(respBody.toJson());
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
        fcb(resp);
    } else {
        req->getAttributes()->insert("page", page);
        req->getAttributes()->insert("pageSize", pageSize);
        fccb();
    }
}