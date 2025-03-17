#include "RoleController.h"

void RoleController::roleList(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int userId = req->getAttributes()->get<int>("uid");
    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    Mapper<Role> mpRole(db);
    auto loginedUser = mpUser.findOne(Criteria(User::Cols::_id, userId));
    if (loginedUser.getValueOfRole() == 1) {
        // 只有 administrator(id=1) 才能获取角色列表
        Json::Value data;
        auto roles = mpRole.findAll();
        for (const auto &roleInDb : roles) {
            Json::Value role;
            role["id"] = roleInDb.getValueOfId();
            role["name"] = roleInDb.getValueOfName();
            data.append(role);
        }
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success(data).toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}