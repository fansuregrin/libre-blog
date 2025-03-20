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
const std::string ArticleController::articleGetSql = 
R"(SELECT a.id, a.title, a.excerpt, a.content, a.create_time,
    u.id AS user_id, u.username AS user_username, u.realname AS user_realname,
    c.id AS category_id, c.slug AS category_slug, c.name AS category_name,
    t.id AS tag_id, t.slug AS tag_slug, t.name AS tag_name
FROM article a
LEFT JOIN user u ON a.author = u.id
LEFT JOIN category c ON a.category = c.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id
WHERE a.id = ?;)";
const std::string ArticleController::articleInsertSql = 
R"(INSERT INTO article (title, author, category, create_time, content, excerpt) 
VALUE (?, ?, ?, ?, ?, ?);)";

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

    Json::Value data;
    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");

    auto roleId = db->execSqlSync("SELECT role FROM user WHERE id = ?", userId)
        [0][0].as<int>();
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
    Json::Value article = resultToArticle(db->execSqlSync(articleGetSql, id));   
    auto resp = HttpResponse::newHttpJsonResponse(
        ApiResponse::success(article).toJson());
    callback(resp);
}

void ArticleController::addArticle(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    Article article
) const {
    int userId = req->getAttributes()->get<int>("uid");
    auto db = app().getDbClient();
    
    int roleId= db->execSqlSync("SELECT role FROM user WHERE id = ?", userId)
        [0][0].as<int>();
    if (roleId > 0 && roleId <= 3) {
        // administrator(id=1),editor(id=2),contributor(id=3)有新增文章的权限
        const auto &reqData = *req->getJsonObject();
        
        // 插入文章
        if (article.categoryId < 1) {
            article.categoryId = 1;
        }
        auto result = db->execSqlSync(articleInsertSql, 
            article.title, article.authorId, article.categoryId,
            article.createTime, article.content, article.excerpt);
        
        // 依次处理前端传过来的每一个tag
        for (auto &tag : article.tags) {
            auto result = db->execSqlSync(
                "SELECT id FROM tag WHERE name = ?", tag.name);
            if (result.empty()) {
                auto r = db->execSqlSync(
                    "INSERT INTO tag (slug, name) VALUE (?, ?)", tag.name, tag.name);
                tag.id = r.insertId();
            } else {
                tag.id = result[0][0].as<int>();
            }
            db->execSqlSync("INSERT INTO article_tag (article, tag) VALUE (?, ?)",
                article.id, tag.id);
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
    bool hasPermission = false;
    int userId = req->getAttributes()->get<int>("uid");
    auto db = app().getDbClient();
    
    auto authorId = db->execSqlSync("SELECT author FROM article WHERE id = ?", article.id)
        [0][0].as<int>();
    auto roleId = db->execSqlSync("SELECT role FROM user WHERE id = ?", userId)
        [0][0].as<int>();
    if (roleId <= 2) {
        // administrator 和 editor 有更新任意文章的权限
        hasPermission = true;
    } else if (roleId == 3) {
        // contributor 只能更新自己写的文章
        hasPermission = (userId == authorId);
    }

    if (hasPermission) {
        // 更新文章
        std::vector<std::string> fields;
        fields.emplace_back("title = " + article.title);
        fields.emplace_back("excerpt = " + article.excerpt);
        fields.emplace_back("content = " + article.content);
        if (article.authorId > 0) {
            fields.emplace_back("author = " + std::to_string(article.authorId));
        }
        if (article.categoryId > 0) {
            fields.emplace_back("category = " + std::to_string(article.categoryId));
        }
        std::string updateSql = "UPDATE article SET " + join(fields, "", "", ",")
            + " WHERE id = ?";
        db->execSqlSync(updateSql, article.id);

        // 清除 article 对应的所有旧的 tag
        db->execSqlSync("DELETE FROM article_tag WHERE article = ?", article.id);
        
        // 依次处理前端传过来的每一个tag
        for (auto &tag : article.tags) {
            auto result = db->execSqlSync(
                "SELECT id FROM tag WHERE name = ?", tag.name);
            if (result.empty()) {
                auto r = db->execSqlSync(
                    "INSERT INTO tag (slug, name) VALUE (?, ?)", tag.name, tag.name);
                tag.id = r.insertId();
            } else {
                tag.id = result[0][0].as<int>();
            }
            db->execSqlSync("INSERT INTO article_tag (article, tag) VALUE (?, ?)",
                article.id, tag.id);
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
    auto idListSql = join(idList, "(", ")", ",");

    auto db = app().getDbClient();

    auto roleId = db->execSqlSync("SELECT role FROM user WHERE id = ?", userId)
        [0][0].as<int>();
    if (roleId >= 4) {
        throw PermissionException();
    } else if (roleId == 3) {
        // contributor(id=3) 只能删除自己写的文章
        auto res = db->execSqlSync("SELECT id FROM article WHERE id IN ? AND author = ?",
            idListSql, userId);
        idList.clear();
        for (const auto &row : res) {
            idList.emplace_back(row[0].as<int>());
        }
        if (!idList.empty()) {
            idListSql = join(idList, "(", ")", ",");
            db->execSqlSync("DELETE FROM article WHERE id IN ?", idListSql);
            db->execSqlSync("DELETE FROM article_tag WHERE article IN ?", idListSql);
        }
    } else if (roleId == 1 || roleId == 2) {
        // administrator(id=1) 和 editor(id=2) 有删除任意文章的权限
        if (!idListSql.empty()) {
            db->execSqlSync("DELETE FROM article WHERE id IN ?", idListSql);
            db->execSqlSync("DELETE FROM article_tag WHERE article IN ?", idListSql);
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

Json::Value ArticleController::resultToArticle(const Result &result) {
    Json::Value article;
    bool first = true;
    for (const auto &row : result) {
        if (first) {
            first = false;
            article["id"] = row["id"].as<int>();
            article["title"] = row["title"].as<std::string>();
            article["excerpt"] = row["excerpt"].as<std::string>();
            article["content"] = row["content"].as<std::string>();
            article["create_time"] = row["create_time"].as<std::string>();
            article["author"]["id"] = row["user_id"].as<int>();
            article["author"]["username"] = row["user_username"].as<std::string>();
            article["author"]["realname"] = row["user_realname"].as<std::string>();
            article["category"]["id"] = row["category_id"].as<int>();
            article["category"]["slug"] = row["category_slug"].as<std::string>();
            article["category"]["name"] = row["category_name"].as<std::string>();
        }
        if (!row["tag_id"].isNull()) {
            Json::Value tag;
            tag["id"] = row["tag_id"].as<int>();
            tag["slug"] = row["tag_slug"].as<std::string>();
            tag["name"] = row["tag_name"].as<std::string>();
            article["tags"].append(tag);
        }
    }
    return article;
}