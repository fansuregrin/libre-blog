#include <drogon/drogon.h>


int main() {
    drogon::app().setExceptionHandler(
        [] (
            const std::exception &ex,
            const drogon::HttpRequestPtr &req,
            std::function<void(const drogon::HttpResponsePtr &)> &&callback
        ) {
            Json::Value json;
            if (typeid(ex) == typeid(std::invalid_argument) ||
            typeid(ex) == typeid(std::runtime_error)) {
                json["status"] = 2;
                json["error"] = ex.what();
            } else if (typeid(ex) == typeid(drogon::orm::DrogonDbException)) {
                json["status"] = 2;
                json["error"] = "操作数据库出现异常";
            } else {
                json["status"] = 2;
                LOG_ERROR << ex.what();
            }
            auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        }
    );

    drogon::app().loadConfigFile("./config.json");
    drogon::app().run();
    return 0;
}
