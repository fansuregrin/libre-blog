#include "RoleController.h"

void RoleController::roleList(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int userId = req->getAttributes()->get<int>("uid");
    int roleId = UserMapper::selectRoleId(userId);
    // 只有 administrator(id=1) 才能获取角色列表
    if (roleId == Role::ADMINISTRATOR) {
        auto roles = RoleMapper::selectAll();
        Json::Value data = toJson(roles);
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success(data).toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}