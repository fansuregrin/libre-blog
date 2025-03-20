#include "MenuController.h"

void MenuController::getMenuAdmin(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int userId = req->getAttributes()->get<int>("uid");
    // todo
    Json::Value data;
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void MenuController::getMenu(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    // todo
    Json::Value data;
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}
