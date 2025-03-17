#include "TagController.h"

void TagController::getAllTags(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const { 
    auto db = app().getDbClient();
    Mapper<Tag> mpTag(db);
    Mapper<ArticleTag> mpArticleTag(db);
    Json::Value data;
    auto numTags = mpTag.count();
    auto tags = mpTag.findAll();
    for (const auto tagInDb : tags) {
        Json::Value tag;
        tag["name"] = tagInDb.getValueOfName();
        tag["slug"] = tagInDb.getValueOfSlug();
        data.append(tag);
    }
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void TagController::tagList(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int page
) const {
    if (page < 1) {
        throw PageException();
    }

    auto db = app().getDbClient();
    Mapper<Tag> mpTag(db);
    Mapper<ArticleTag> mpArticleTag(db);
    Json::Value data;
    auto numTags = mpTag.count();
    int perPage = 10;
    data["num_pages"] = numTags / perPage + (numTags%perPage?1:0);
    auto tags = mpTag.paginate(page, perPage).findAll();
    for (const auto tagInDb : tags) {
        Json::Value tag;
        tag["id"] = tagInDb.getValueOfId();
        tag["name"] = tagInDb.getValueOfName();
        tag["slug"] = tagInDb.getValueOfSlug();
        tag["num_articles"] = mpArticleTag.count(
            Criteria(ArticleTag::Cols::_tag, tagInDb.getValueOfId()));
        data["tags"].append(tag);
    }
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
    auto db = app().getDbClient();
    Mapper<Tag> mpTag(db);
    auto tagInDb = mpTag.findOne(Criteria(Tag::Cols::_id, id));
    Json::Value data;
    data["id"] = tagInDb.getValueOfId();
    data["name"] = tagInDb.getValueOfName();
    data["slug"] = tagInDb.getValueOfSlug();
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(data).toJson()
    );
    callback(resp);
}

void TagController::addTag(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    int userId = req->getAttributes()->get<int>("uid");

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    Mapper<Tag> mpTag(db);

    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    if (userInDb.getValueOfRole() <= 2) {
        // 只有 administrator(id=1) 和 editor(id=2) 才有新增标签的权限
        auto &reqJson = *req->getJsonObject();
        if (!reqJson.isMember("name") || 
        reqJson["name"].type() != Json::stringValue) {
            throw std::invalid_argument("缺少必备字段: name, 或者类型错误");
        }
        if (!reqJson.isMember("slug") || 
        reqJson["slug"].type() != Json::stringValue) {
            throw std::invalid_argument("缺少必备字段: slug, 或者类型错误");
        }
        Tag tag(reqJson);
        mpTag.insert(tag);
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
    const Tag &tag
) const {
    int userId = req->getAttributes()->get<int>("uid");

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    Mapper<Tag> mpTag(db);


    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    if (userInDb.getValueOfRole() <= 2) {
        // 只有 administrator(id=1) 和 editor(id=2) 才有更新标签的权限
        mpTag.update(tag);
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

    std::vector<int> idList;
    for (const auto &id : reqJson["ids"]) {
        if (id.asInt() > 1) {
            idList.emplace_back(id.asInt());
        }
    }

    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    Mapper<Tag> mpTag(db);
    Mapper<ArticleTag> mpArticleTag(db);

    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    if (userInDb.getValueOfRole() <= 2) {
        // 只有 administrator(id=1) 和 editor(id=2) 才有删除标签的权限
        if (!idList.empty()) {
            mpArticleTag.deleteBy(
                Criteria(ArticleTag::Cols::_tag, CompareOperator::In, idList));
            mpTag.deleteBy(Criteria(Tag::Cols::_id, CompareOperator::In, idList)); 
        }
        auto resp = HttpResponse::newHttpJsonResponse(
            ApiResponse::success().toJson()
        );
        callback(resp);
    } else {
        throw PermissionException();
    }
}