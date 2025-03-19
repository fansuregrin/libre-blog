#include "CategoryController.h"

void CategoryController::getAllCategories(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    Mapper<Category> mp(app().getDbClient());
    auto categoires = mp.findBy(
        Criteria(Category::Cols::_id, CompareOperator::GT, 1));
    Json::Value data;
    for (const auto &cat : categoires) {
        Json::Value item;
        item["name"] = cat.getValueOfName();
        item["id"] = cat.getValueOfId();
        item["slug"] = cat.getValueOfSlug();
        data.append(item);
    }
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
    auto db = app().getDbClient();
    Mapper<Category> mpCategory(db);
    auto catInDb = mpCategory.findOne(Criteria(Category::Cols::_id, id));
    Json::Value data = catInDb.toJson();
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void CategoryController::getCategoryBySlug(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    std::string slug
) const {
    auto db = app().getDbClient();
    Mapper<Category> mpCategory(db);
    auto catInDb = mpCategory.findOne(Criteria(Category::Cols::_slug, slug));
    Json::Value data = catInDb.toJson();
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void CategoryController::addCategory(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int userId = req->getAttributes()->get<int>("uid");

    auto db = app().getDbClient();
    Mapper<Category> mpCategory(db);
    Mapper<User> mpUser(db);
    Mapper<Role> mpRole(db);

    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    if (userInDb.getValueOfRole() <= 2) {
        // 只有 administrator(id=1) 和 editor(id=2) 才有新增分类的权限
        auto &reqJson = *req->getJsonObject();
        if (!reqJson.isMember("name") || 
        reqJson["name"].type() != Json::stringValue) {
            throw std::invalid_argument("缺少必备字段: name, 或者类型错误");
        }
        if (!reqJson.isMember("slug") || 
        reqJson["slug"].type() != Json::stringValue) {
            throw std::invalid_argument("缺少必备字段: slug, 或者类型错误");
        }

        Category cat(reqJson);
        mpCategory.insert(cat);
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
    const Category &cat
) const {
    int userId = req->getAttributes()->get<int>("uid");

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    Mapper<Category> mpCategory(db);

    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    if (userInDb.getValueOfRole() <= 2) {
        // 只有 administrator(id=1) 和 editor(id=2) 有更新分类的权限
        mpCategory.update(cat);
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
    Mapper<Article> mpArticle(db);
    Mapper<Category> mpCategory(db);
    Mapper<User> mpUser(db);

    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    if (userInDb.getValueOfRole() <= 2) {
        // 只有 administrator(id=1) 和 editor(id=2) 有删除分类的权限
        if (!idList.empty()) {
            // 将要删除的分类下面的文章的分类id改成保留分类的id（即id为1的分类）
            mpArticle.updateBy(
                {Article::Cols::_category},
                Criteria(Article::Cols::_category, CompareOperator::In, idList),
                1);
            // 更新了文章的category后才能删除这些分类
            mpCategory.deleteBy(
                Criteria(Category::Cols::_id, CompareOperator::In, idList));
        }
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}
