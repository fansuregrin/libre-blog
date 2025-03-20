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
    auto db = app().getDbClient();
    auto row = db->execSqlSync("SELECT id,slug,name FROM category WHERE slug = ?", slug)[0];
    Json::Value data;
    data["id"] = row["id"].as<int>();
    data["slug"] = row["slug"].as<std::string>();
    data["name"] = row["name"].as<std::string>();
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
    auto db = app().getDbClient();

    auto roleId = db->execSqlSync("SELECT role FROM user WHERE id = ?", userId)
        [0][0].as<int>();
    if (roleId <= 2) {
        // 只有 administrator(id=1) 和 editor(id=2) 才有新增分类的权限
        db->execSqlSync("INSERT INTO category (slug, name) VALUE (?, ?)",
            category.slug, category.name);
        
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
    auto db = app().getDbClient();
    auto roleId = db->execSqlSync("SELECT role FROM user WHERE id = ?", userId)
        [0][0].as<int>();
    if (roleId <= 2) {
        // 只有 administrator(id=1) 和 editor(id=2) 有更新分类的权限
        db->execSqlSync("UPDATE category SET slug = ?, name = ?",
            category.slug, category.name);
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

    std::vector<int> idList;
    for (const auto &id : reqJson["ids"]) {
        if (id.asInt() > 1) {
            idList.emplace_back(id.asInt());
        }
    }
    
    auto db = app().getDbClient();

    auto roleId = db->execSqlSync("SELECT role FROM user WHERE id = ?", userId)
        [0][0].as<int>();
    if (roleId <= 2) {
        // 只有 administrator(id=1) 和 editor(id=2) 有删除分类的权限
        if (!idList.empty()) {
            std::string idListSql = join(idList, "(", ")", ",");
            // 将要删除的分类下面的文章的分类id改成保留分类的id（即id为1的分类）
            db->execSqlSync("UPDATE article SET category = 1 WHERE category IN ?", idListSql);
            // 删除分类
            db->execSqlSync("DELETE FROM category WHERE id IN ?", idListSql);
        }
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}
