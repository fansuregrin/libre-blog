#include "ArticleController.h"

void ArticleController::articleList(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");
    Json::Value data;

    data["total"] = ArticleMapper::count();
    auto articles = ArticleMapper::selectLimit(page, pageSize);
    data["articles"] = toJson(articles);
    
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void ArticleController::articleListAdmin(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int userId = req->getAttributes()->get<int>("uid");
    auto db = drogon::app().getDbClient();

    Json::Value data;
    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");

    auto roleId = UserMapper::selectRoleId(userId);
    if (roleId == Role::ADMINISTRATOR || roleId == Role::EDITOR) {
        data["total"] = ArticleMapper::count();
        auto articles = ArticleMapper::selectLimit(page, pageSize);
        data["articles"] = toJson(articles);
    } else if (roleId == Role::CONTRIBUTOR) {
        data["total"] = ArticleMapper::countByUser(userId);
        auto articles = ArticleMapper::selectLimitByUser(userId, page, pageSize);
        data["articles"] = toJson(articles);
    } else {
        throw std::runtime_error("无效的 role id");
    }

    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void ArticleController::articleListByCategory(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const std::string &slug
) const {
    Json::Value data;
    auto db = app().getDbClient();
    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");

    data["total"] = ArticleMapper::countByCategory(slug);
    auto articles = ArticleMapper::selectLimitByCategory(slug, page, pageSize);
    data["articles"] = toJson(articles);

    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void ArticleController::articleListByAuthor(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int id
) const {
    Json::Value data;
    auto db = app().getDbClient();
    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");

    data["total"] = ArticleMapper::countByUser(id);
    auto articles = ArticleMapper::selectLimitByUser(id, page, pageSize);
    data["articles"] = toJson(articles);

    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void ArticleController::articleListByTag(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const std::string &slug
) const {
    Json::Value data;
    auto db = app().getDbClient();
    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");

    data["total"] = ArticleMapper::countByTag(slug);
    auto articles = ArticleMapper::selectLimitByTag(slug, page, pageSize);
    data["articles"] = toJson(articles);

    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void ArticleController::getArticle(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int id
) const {
    auto db = app().getDbClient();
    auto article = ArticleMapper::select(id);
    Json::Value data = article ? article->toJson() : Json::nullValue;
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson());
    callback(resp);
}

void ArticleController::addArticle(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    Article article
) const {
    int userId = req->getAttributes()->get<int>("uid");
    auto db = app().getDbClient();
    
    int roleId= UserMapper::selectRoleId(userId);
    // administrator(id=1),editor(id=2),contributor(id=3)有新增文章的权限
    if (roleId == Role::ADMINISTRATOR || roleId == Role::EDITOR || 
        roleId == Role::CONTRIBUTOR) {
        // 插入文章
        article.authorId = userId;
        if (article.categoryId < Category::UNCATEGORIED) {
            article.categoryId = Category::UNCATEGORIED;
        }
        ArticleMapper::insert(article);
        
        // 依次处理前端传过来的每一个tag
        for (auto &tag : article.tags) {
            auto tagInDb = TagMapper::selectByName(tag.name);
            if (!tagInDb) {
                if (tag.slug.empty()) tag.slug = tag.name;
                TagMapper::insert(tag);
            } else {
                tag.id = tagInDb->id;
            }
            ArticleTag at{article.id, tag.id};
            ArticleTagMapper::insert(at);
        }
        
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}

void ArticleController::updateArticle(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    Article article
) const {
    int userId = req->getAttributes()->get<int>("uid");
    auto db = app().getDbClient();
    
    auto oldArticle = ArticleMapper::select(article.id);
    auto roleId = UserMapper::selectRoleId(userId);
    bool hasPermission = false;
    if (roleId == Role::ADMINISTRATOR || roleId == Role::EDITOR) {
        // administrator 和 editor 有更新任意文章的权限
        hasPermission = true;
    } else if (roleId == Role::CONTRIBUTOR) {
        // contributor 只能更新自己写的文章
        hasPermission = (userId == oldArticle->authorId);
    }

    if (hasPermission) {
        // 更新文章
        if (roleId == Role::CONTRIBUTOR) article.authorId = -1;
        ArticleMapper::update(article);

        // 清除 article 对应的所有旧的 tag
        ArticleTagMapper::deleteByArticle(article.id);
        
        // 依次处理前端传过来的每一个tag
        for (auto &tag : article.tags) {
            auto tagInDb = TagMapper::selectByName(tag.name);
            if (!tagInDb) {
                if (tag.slug.empty()) tag.slug = tag.name;
                TagMapper::insert(tag);
            } else {
                tag.id = tagInDb->id;
            }
            ArticleTag at{article.id, tag.id};
            ArticleTagMapper::insert(at);
        }
        
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}

void ArticleController::deleteArticles(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    auto pJson = req->getJsonObject();
    if (!pJson) {
        throw std::invalid_argument("请求体格式错误, 请使用json");
    }
    const auto &reqJson = *pJson;
    if (!reqJson.isMember("ids") || reqJson["ids"].type() != Json::arrayValue) {
        throw std::invalid_argument("缺少必备字段: ids, 或者类型错误");
    }
    
    int userId = req->getAttributes()->get<int>("uid");
    
    std::vector<int> ids;
    for (const auto &id : reqJson["ids"]) {
        ids.emplace_back(id.asInt());
    }
    auto idListSql = join(ids, "(", ")", ",");

    auto db = app().getDbClient();

    auto roleId = UserMapper::selectRoleId(userId);
    if (roleId >= Role::SUBSCRIBER) {
        throw PermissionException();
    } else if (roleId == 3) {
        // contributor(id=3) 只能删除自己写的文章
        ArticleMapper::deletes(ids, userId);
        ArticleTagMapper::deleteByArticles(ids, userId);
    } else if (roleId == Role::ADMINISTRATOR || roleId == Role::EDITOR) {
        // administrator(id=1) 和 editor(id=2) 有删除任意文章的权限
        ArticleMapper::deletes(ids);
        ArticleTagMapper::deleteByArticles(ids);
    } else {
        throw std::runtime_error("无效的 role id");
    }

    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success().toJson()
    );
    resp->setStatusCode(HttpStatusCode::k204NoContent);
    callback(resp);
}