#include "CategoryController.h"

void CategoryController::getAllCategories(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    auto categories = CategoryMapper::selectAll();
    Json::Value data = toJson(categories);
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void CategoryController::getCategory(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int id
) const {
    auto category = CategoryMapper::select(id);
    Json::Value data = category ? category->toJson() : Json::nullValue;
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void CategoryController::getCategoryBySlug(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const std::string &slug
) const {
    auto category = CategoryMapper::selectBySlug(slug);
    Json::Value data = category ? category->toJson() : Json::nullValue;
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void CategoryController::addCategory(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    Category category
) const {
    int userId = req->getAttributes()->get<int>("uid");
    auto roleId = UserMapper::selectRoleId(userId);
    // 只有 administrator(id=1) 和 editor(id=2) 才有新增分类的权限
    if (roleId <= Role::EDITOR) {
        CategoryMapper::insert(category);
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}

void CategoryController::updateCategory(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    Category category
) const {
    int userId = req->getAttributes()->get<int>("uid");
    auto roleId = UserMapper::selectRoleId(userId);
    // 只有 administrator(id=1) 和 editor(id=2) 有更新分类的权限
    if (roleId <= Role::EDITOR) {
        CategoryMapper::update(category);
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}

void CategoryController::deleteCategories(
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
        if (id.asInt() > 1) {
            ids.emplace_back(id.asInt());
        }
    }
    
    auto db = app().getDbClient();

    auto roleId = UserMapper::selectRoleId(userId);
    // 只有 administrator(id=1) 和 editor(id=2) 有删除分类的权限
    if (roleId <= Role::EDITOR) {
        if (!ids.empty()) {
            CategoryMapper::deltes(ids);
            ArticleMapper::updateCategoryToDefault(ids);
        }
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}
