#include "MenuController.h"

void MenuController::getMenuAdmin(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    Json::Value data;
    int userId = req->getAttributes()->get<int>("uid");
    auto menus = MenuMapper::selectByUser(userId);
    data = toJson(menus);
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}
