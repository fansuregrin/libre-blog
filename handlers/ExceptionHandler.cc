#include "ExceptionHandler.h"
#include "../dtos/ApiResponse.h"
#include "../exceptions/PageException.h"
#include "../exceptions/PermissionException.h"

using namespace drogon;

ExceptionHandler exceptionHandler = [] (
    const std::exception &ex,
    const HttpRequestPtr &req,
    std::function<void(const HttpResponsePtr &)> &&callback
) {
    LOG_DEBUG << ex.what();
    ApiResponse respBody;
    auto resp = HttpResponse::newHttpResponse();
    resp->setContentTypeCode(ContentType::CT_APPLICATION_JSON);
    
    const auto *dbEx = dynamic_cast<const drogon::orm::DrogonDbException *>(&ex);
    if (dbEx) {
        respBody.setCodeAndMsg(1, "数据操作出错");
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
    } else if (typeid(ex) == typeid(PageException)) {
        respBody.setCodeAndMsg(1, "无效的页码");
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
    } else if (typeid(ex) == typeid(PermissionException)) {
        respBody.setCodeAndMsg(1, "没有权限");
        resp->setStatusCode(HttpStatusCode::k403Forbidden);
    } else if (typeid(ex) == typeid(std::invalid_argument) ||
    typeid(ex) == typeid(std::runtime_error)) {
        respBody.setCodeAndMsg(1, ex.what());
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
    } else {
        respBody.setCodeAndMsg(1, "未知错误");
        resp->setStatusCode(HttpStatusCode::k400BadRequest);
    }

    Json::StreamWriterBuilder writer;
    resp->setBody(Json::writeString(writer, respBody.toJson()));
    callback(resp);
};