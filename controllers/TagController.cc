#include "TagController.h"

void TagController::getAllTags(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    auto tags = TagMapper::selectAll();
    Json::Value data = toJson(tags);
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void TagController::tagList(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");
    
    Json::Value data;
    data["total"] = TagMapper::count();
    auto tags = TagMapper::selectLimit(page, pageSize);
    data["tags"] = toJson(tags);

    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void TagController::getTag(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int id
) const {
    auto tag = TagMapper::selectById(id);
    Json::Value data = tag->toJson();
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void TagController::addTag(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    Tag tag
) const {
    int userId = req->getAttributes()->get<int>("uid");
    int roleId = UserMapper::selectRoleId(userId);
    if (roleId <= Role::EDITOR) {
        // 只有 administrator(id=1) 和 editor(id=2) 才有新增标签的权限
        TagMapper::insert(tag);
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}

void TagController::updateTag(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    Tag tag
) const {
    int userId = req->getAttributes()->get<int>("uid");
    int roleId = UserMapper::selectRoleId(userId);
    if (roleId <= Role::EDITOR) {
        // 只有 administrator(id=1) 和 editor(id=2) 才有更新标签的权限
        TagMapper::update(tag);
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}

void TagController::deleteTags(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int userId = req->getAttributes()->get<int>("uid");

    const auto &reqJson = *req->getJsonObject();
    if (!reqJson.isMember("ids") || reqJson["ids"].type() != Json::arrayValue) {
        throw std::invalid_argument("缺少必备字段: ids, 或者类型错误");
    }
    std::vector<int> ids;
    for (const auto &id : reqJson["ids"]) {
        ids.emplace_back(id.asInt());
    }

    int roleId = UserMapper::selectRoleId(userId);
    if (roleId <= Role::EDITOR) {
        // 只有 administrator(id=1) 和 editor(id=2) 才有删除标签的权限
        TagMapper::deletes(ids);
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}