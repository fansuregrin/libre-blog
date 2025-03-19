#include "ArticleController.h"

const std::string ArticleController::articleListSql = 
R"(SELECT a.id, a.title, a.excerpt, a.create_time,
    u.id AS user_id, u.username AS user_username, u.realname AS user_realname,
    c.id AS category_id, c.slug AS category_slug, c.name AS category_name,
    t.id AS tag_id, t.slug AS tag_slug, t.name AS tag_name
FROM (SELECT id FROM article ORDER BY create_time DESC LIMIT ?,?) sub
JOIN article a ON sub.id = a.id
LEFT JOIN category c ON a.category = c.id
LEFT JOIN user u ON a.author = u.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id;)";
const std::string ArticleController::articleCountSql = 
"SELECT COUNT(*) FROM article";
const std::string ArticleController::articleListByUserSql =
R"(SELECT a.id, a.title, a.excerpt, a.create_time,
    u.id AS user_id, u.username AS user_username, u.realname AS user_realname,
    c.id AS category_id, c.slug AS category_slug, c.name AS category_name,
    t.id AS tag_id, t.slug AS tag_slug, t.name tag_name
FROM (SELECT id FROM article WHERE author = ? ORDER BY create_time DESC LIMIT ?,?) sub
JOIN article a ON sub.id = a.id
LEFT JOIN category c ON a.category = c.id
JOIN user u ON a.author = u.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id;)";
const std::string ArticleController::articleCountByUserSql = 
"SELECT COUNT(*) FROM article WHERE author = ?";
const std::string ArticleController::articleListByCategorySql =
R"(SELECT a.id, a.title, a.excerpt, a.create_time,
    u.id AS user_id, u.username AS user_username, u.realname AS user_realname,
    c.id AS category_id, c.slug AS category_slug, c.name AS category_name,
    t.id AS tag_id, t.slug AS tag_slug, t.name AS tag_name
FROM (SELECT a1.id FROM article a1 LEFT JOIN category c1 ON a1.category = c1.id
    WHERE c1.slug = ? ORDER BY a1.create_time DESC LIMIT ?,?) sub
JOIN article a ON sub.id = a.id
JOIN category c ON a.category = c.id
LEFT JOIN user u ON a.author = u.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id;)";
const std::string ArticleController::articleCountByCategorySql =
"SELECT COUNT(*) FROM article a JOIN category c ON a.category = c.id WHERE c.slug = ?";
const std::string ArticleController::articleListByTagSql =
R"(SELECT a.id, a.title, a.excerpt, a.create_time,
    u.id AS user_id, u.username AS user_username, u.realname AS user_realname,
    c.id AS category_id, c.slug AS category_slug, c.name AS category_name,
    t.id AS tag_id, t.slug AS tag_slug, t.name AS tag_name
FROM (SELECT a1.id FROM article a1
    JOIN article_tag at1 ON a1.id = at1.article
    JOIN tag t1 ON at1.tag = t1.id WHERE t1.slug = ?
    ORDER BY a1.create_time DESC LIMIT ?,?) sub
JOIN article a ON sub.id = a.id
LEFT JOIN category c ON a.category = c.id
LEFT JOIN user u ON a.author = u.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id;)";
const std::string ArticleController::articleCountByTagSql =
R"(SELECT COUNT(*) FROM article a JOIN article_tag at ON a.id = at.article
JOIN tag t ON at.tag = t.id WHERE t.slug = ?)";

void ArticleController::articleList(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    Json::Value data;
    auto db = drogon::app().getDbClient();
    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");

    data["total"] = db->execSqlSync(articleCountSql)[0][0].as<int>();
    auto result = db->execSqlSync(articleListSql, (page-1)*pageSize, pageSize);
    data["articles"] = resultToArticles(result);
    
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
    Mapper<User> mpUser(db);

    Json::Value data;
    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");
    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    auto roleId = userInDb.getValueOfRole();
    
    if (roleId <= 2) {
        data["total"] = db->execSqlSync(articleCountSql)[0][0].as<int>();
        auto result = db->execSqlSync(articleListSql, (page-1)*pageSize, pageSize);
        data["articles"] = resultToArticles(result);
    } else if (roleId == 3) {
        data["total"] = db->execSqlSync(articleCountByUserSql, userId)[0][0].as<int>();
        auto result = db->execSqlSync(articleListByUserSql, 
            userId, (page-1)*pageSize, pageSize);
        data["articles"] = resultToArticles(result);
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

    data["total"] = db->execSqlSync(articleCountByCategorySql, slug)[0][0].as<int>();
    auto result = db->execSqlSync(articleListByCategorySql, slug,
        (page-1)*pageSize, pageSize);
    data["articles"] = resultToArticles(result);

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

    data["total"] = db->execSqlSync(articleCountByUserSql, id)[0][0].as<int>();
    auto result = db->execSqlSync(articleListByUserSql, id,
        (page-1)*pageSize, pageSize);
    data["articles"] = resultToArticles(result);

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

    data["total"] = db->execSqlSync(articleCountByTagSql, slug)[0][0].as<int>();
    auto result = db->execSqlSync(articleListByTagSql, slug, (page-1)*pageSize, pageSize);
    data["articles"] = resultToArticles(result);

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
    Mapper<Article> mpArticle(db);
    auto art = mpArticle.findOne({Article::Cols::_id, id});
    Json::Value article;
    article["id"] = art.getValueOfId();
    article["title"] = art.getValueOfTitle();
    Json::Value author;
    auto authorInDb = art.getUser(db);
    author["id"] = authorInDb.getValueOfId();
    author["realname"] = authorInDb.getValueOfRealname();
    article["author"] = author;
    article["category"] = art.getCategory(db).toJson();
    auto tags = art.getTags(db);
    for (const auto &tag : tags) {
        article["tags"].append(tag.first.toJson());
    }
    article["create_time"] = art.getValueOfCreateTime()
        .toCustomFormattedString("%Y-%m-%dT%H:%M:%SZ");
    article["content"] = art.getValueOfContent();
    article["excerpt"] = art.getValueOfExcerpt();
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(article).toJson());
    callback(resp);
}

void ArticleController::addArticle(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const std::vector<std::string> &tags
) const {
    int userId = req->getAttributes()->get<int>("uid");
    auto db = app().getDbClient();
    Mapper<Article> mpArticle(db);
    Mapper<Tag> mpTag(db);
    Mapper<ArticleTag> mpArticleTag(db);
    Mapper<User> mpUser(db);
    
    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    if (userInDb.getValueOfRole() <= 3) {
        // administrator(id=1),editor(id=2),contributor(id=3)有新增文章的权限
        Article art;
        art.updateByJson(*req->getJsonObject());
        art.setAuthor(userId);
        if (art.getCategory() == nullptr) {
            art.setCategory(1);
        }
        mpArticle.insert(art);
        
        // 依次处理前端传过来的每一个tag
        for (const auto &tagName : tags) {
            Tag tag;
            // 查询表Tag中有没有指定name字段的tag
            auto cnt = mpTag.count(Criteria(Tag::Cols::_name, tagName));
            if (cnt <= 0) {
                // 如果没有，则需要向表Tag中插入新的tag
                tag.setName(tagName);
                tag.setSlug(tagName);
                mpTag.insert(tag);
            } else {
                // 如果有，则需要从表Tag中获取这个tag
                tag = mpTag.findOne(Criteria(Tag::Cols::_name, tagName));
            }
            // 然后，向表ArticleTag中插入此tag和article的对应关系
            ArticleTag artTag;
            artTag.setArticle(art.getValueOfId());
            artTag.setTag(tag.getValueOfId());
            mpArticleTag.insert(artTag);
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
    const Article &article,
    const std::vector<std::string> &tags
) const {
    bool hasPermission = false;
    int userId = req->getAttributes()->get<int>("uid");
    auto db = app().getDbClient();
    Mapper<Article> mpArticle(db);
    Mapper<Tag> mpTag(db);
    Mapper<ArticleTag> mpArticleTag(db);
    Mapper<User> mpUser(db);
    
    auto artInDb = mpArticle.findOne({Article::Cols::_id, article.getValueOfId()});
    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    auto roleId = userInDb.getValueOfRole();
    if (roleId <= 2) {
        // administrator 和 editor 有更新任意文章的权限
        hasPermission = true;
    } else if (roleId == 3) {
        // contributor 只能更新自己写的文章
        hasPermission = (userId == artInDb.getValueOfAuthor());
    }

    if (hasPermission) {
        mpArticle.updateBy(
            {Article::Cols::_title, Article::Cols::_category,
                Article::Cols::_excerpt, Article::Cols::_content},
            {Article::Cols::_id, article.getValueOfId()},
            article.getValueOfTitle(), article.getValueOfCategory(),
            article.getValueOfExcerpt(), article.getValueOfContent()
        );
        // 清除article对应的所有旧的tag
        mpArticleTag.deleteBy(
            Criteria(ArticleTag::Cols::_article, CompareOperator::EQ, article.getValueOfId()));
        // 依次处理前端传过来的每一个tag
        for (const auto &tagName : tags) {
            // 查询表Tag中有没有指定name字段的tag
            auto cnt = mpTag.count(Criteria(Tag::Cols::_name, CompareOperator::EQ, tagName));
            if (cnt <= 0) {
                // 如果没有，则需要向表Tag中插入新的tag
                Tag tag;
                tag.setName(tagName);
                tag.setSlug(tagName);
                mpTag.insert(tag);
                // 然后，向表ArticleTag中插入此tag和article的对应关系
                ArticleTag artTag;
                artTag.setArticle(article.getValueOfId());
                artTag.setTag(tag.getValueOfId());
                mpArticleTag.insert(artTag);
            } else {
                // 如果有，则需要从表Tag中获取这个tag的id
                auto tagInDb = mpTag.findOne(Criteria(Tag::Cols::_name, CompareOperator::EQ, tagName));
                // 再查询表ArticleTag中有没有这个tag和article的对应关系
                auto existCnt = mpArticleTag.count(
                    Criteria(ArticleTag::Cols::_article, CompareOperator::EQ, article.getValueOfId()) &&
                    Criteria(ArticleTag::Cols::_tag, CompareOperator::EQ, tagInDb.getValueOfId())
                );
                if (existCnt <= 0) {
                    // 如果没有，则向表ArticleTag中插入此tag和article的对应关系
                    ArticleTag artTag;
                    artTag.setArticle(article.getValueOfId());
                    artTag.setTag(tagInDb.getValueOfId());
                    mpArticleTag.insert(artTag);
                }
            }
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
    std::vector<int> idList;
    for (const auto &id : reqJson["ids"]) {
        idList.emplace_back(id.asInt());
    }

    auto db = app().getDbClient();
    Mapper<Article> mpArticle(db);
    Mapper<ArticleTag> mpArticleTag(db);
    Mapper<User> mpUser(db);

    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    auto roleId = userInDb.getValueOfRole();
    if (roleId >= 4) {
        throw PermissionException();
    } else if (roleId == 3) {
        // contributor(id=3) 只能删除自己写的文章
        auto articles = mpArticle.findBy(
            Criteria(Article::Cols::_id, CompareOperator::In, idList) &&
            Criteria(Article::Cols::_author, userId));
        std::vector<int> ids;
        for (const auto &art : articles) {
            ids.emplace_back(art.getValueOfId());
        }
        if (!ids.empty()) {
            // 在删除文章之前需要删除文章与标签的关系
            mpArticleTag.deleteBy(
                Criteria(ArticleTag::Cols::_article, CompareOperator::In, ids)
            );
            // 删除article和tag的关系后，才能删除文章
            mpArticle.deleteBy(
                Criteria(Article::Cols::_id, CompareOperator::In, ids)
            );
        }  
    } else if (roleId == 1 || roleId == 2) {
        // administrator(id=1) 和 editor(id=2) 有删除任意文章的权限
        if (!idList.empty()) {
            // 在删除文章之前需要删除文章与标签的关系
            mpArticleTag.deleteBy(
                Criteria(ArticleTag::Cols::_article, CompareOperator::In, idList)
            );
            // 删除article和tag的关系后，才能删除文章
            mpArticle.deleteBy(
                Criteria(Article::Cols::_id, CompareOperator::In, idList)
            );
        }
    } else {
        throw std::runtime_error("无效的 role id");
    }
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success().toJson()
    );
    resp->setStatusCode(HttpStatusCode::k204NoContent);
    callback(resp);
}

Json::Value ArticleController::resultToArticles(const Result &result) {
    Json::Value articles;
    std::unordered_map<int, Json::Value> articlesMap;
    for (const auto &row : result) {
        int id = row["id"].as<int>();
        if (articlesMap.find(id) == articlesMap.end()) {
            Json::Value article;
            article["id"] = id;
            article["title"]= row["title"].as<std::string>();
            article["excerpt"] = row["excerpt"].as<std::string>();
            article["create_time"] = row["create_time"].as<std::string>();
            article["author"]["id"] = row["user_id"].as<int>();
            article["author"]["username"] = row["user_username"].as<std::string>();
            article["author"]["realname"] = row["user_realname"].as<std::string>();
            article["category"]["id"] = row["category_id"].as<int>();
            article["category"]["slug"] = row["category_slug"].as<std::string>();
            article["category"]["name"] = row["category_name"].as<std::string>();
            articlesMap[id] = article;
        }
        if (!row["tag_id"].isNull()) {
            Json::Value tag;
            tag["id"] = row["tag_id"].as<int>();
            tag["slug"] = row["tag_slug"].as<std::string>();
            tag["name"] = row["tag_name"].as<std::string>();
            articlesMap[id]["tags"].append(tag);
        }
    }
    for (const auto &p : articlesMap) {
        articles.append(p.second);
    }
    return articles;
}
