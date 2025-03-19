#include "ArticleController.h"

void ArticleController::articleList(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    Json::Value data;
    auto db = drogon::app().getDbClient();
    Mapper<Article> mpArticle(db);
    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");

    data["total"] = mpArticle.count();
    auto articles = mpArticle
        .orderBy(Article::Cols::_create_time, orm::SortOrder::DESC)
        .paginate(page, pageSize).findAll();
    
    for (const auto &art : articles) {
        Json::Value article;
        article["id"] = art.getValueOfId();
        article["title"] = art.getValueOfTitle();
        auto author = art.getUser(db);
        article["author"]["id"] = author.getValueOfId();
        article["author"]["realname"] = author.getValueOfRealname();
        article["category"] = art.getCategory(db).toJson();
        auto tags = art.getTags(db);
        for (const auto &tag : tags) {
            article["tags"].append(tag.first.toJson());
        }
        article["create_time"] = art.getValueOfCreateTime()
            .toCustomFormattedString("%Y-%m-%dT%H:%M:%SZ");
        article["excerpt"] = art.getValueOfExcerpt();
        data["articles"].append(article);
    }
    
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
    Mapper<Article> mpArticle(db);

    Json::Value data;

    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");

    auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
    auto roleId = userInDb.getValueOfRole();
    std::vector<Article> articles;
    int numArticles = 0;
    
    if (roleId <= 2) {
        numArticles = mpArticle.count();
        articles = mpArticle.orderBy(Article::Cols::_create_time,
            orm::SortOrder::DESC).paginate(page, pageSize).findAll();
    } else if (roleId == 3) {
        numArticles = mpArticle.count(Criteria(Article::Cols::_author, userId));
        articles = mpArticle
            .orderBy(Article::Cols::_create_time,SortOrder::DESC)
            .paginate(page, pageSize)
            .findBy(Criteria(Article::Cols::_author, userId));
    } else {
        throw std::runtime_error("无效的 role id");
    }
    
    data["total"] = numArticles;
    for (const auto &art : articles) {
        Json::Value article;
        article["id"] = art.getValueOfId();
        article["title"] = art.getValueOfTitle();
        article["author"] = *art.getAuthor();
        article["author_name"] = art.getUser(db).getValueOfRealname();
        article["category"] = *art.getCategory();
        article["category_name"] = art.getCategory(db).getValueOfName();
        auto tags = art.getTags(db);
        for (const auto &tag : tags) {
            article["tags"].append(tag.first.getValueOfName());
        }
        article["create_time"] = art.getValueOfCreateTime()
            .toCustomFormattedString("%Y-%m-%dT%H:%M:%SZ");
        article["excerpt"] = art.getValueOfExcerpt();
        data["articles"].append(article);
    }
    auto resp = HttpResponse::newHttpJsonResponse(ApiResponse::success(data).toJson());
    callback(resp);
}

void ArticleController::articleListByCategory(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const std::string &slug
) const {
    Json::Value data;
    auto db = app().getDbClient();
    Mapper<Category> mpCategory(db);
    Mapper<Article> mpArticle(db);

    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");

    auto cat = mpCategory.findOne(Criteria(Category::Cols::_slug, slug));
    data["category"] = cat.toJson();
    data["total"] = mpArticle.count(
        Criteria(Article::Cols::_category, cat.getValueOfId()));
    auto articles = mpArticle
        .orderBy(Article::Cols::_create_time, SortOrder::DESC)
        .paginate(page, pageSize)
        .findBy(Criteria(Article::Cols::_category, cat.getValueOfId()));
    for (const auto &art : articles) {
        Json::Value article;
        article["id"] = art.getValueOfId();
        article["title"] = art.getValueOfTitle();
        article["create_time"] = art.getValueOfCreateTime()
            .toCustomFormattedString("%Y-%m-%dT%H:%M:%SZ");
        article["excerpt"] = art.getValueOfExcerpt();
        auto author = art.getUser(db);
        article["author"]["id"] = author.getValueOfId();
        article["author"]["realname"] = author.getValueOfRealname();
        auto tags = art.getTags(db);
        for (const auto &tag : tags) {
            article["tags"].append(tag.first.toJson());
        }
        data["articles"].append(article);
    }
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
    Mapper<User> mpUser(db);
    Mapper<Article> mpArticle(db);

    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");

    auto author = mpUser.findOne(Criteria(User::Cols::_id, id));
    data["author"]["id"] = author.getValueOfId();
    data["author"]["realname"] = author.getValueOfRealname();
    data["total"] = mpArticle.count(
        Criteria(Article::Cols::_author, author.getValueOfId()));
    auto articles = mpArticle.orderBy(Article::Cols::_create_time, SortOrder::DESC)
        .paginate(page, pageSize)
        .findBy(Criteria(Article::Cols::_author, author.getValueOfId()));
    for (const auto &art : articles) {
        Json::Value article;
        article["id"] = art.getValueOfId();
        article["title"] = art.getValueOfTitle();
        article["create_time"] = art.getValueOfCreateTime()
            .toCustomFormattedString("%Y-%m-%dT%H:%M:%SZ");
        article["excerpt"] = art.getValueOfExcerpt();
        article["category"] = art.getCategory(db).toJson();
        auto tags = art.getTags(db);
        for (const auto &tag : tags) {
            article["tags"].append(tag.first.toJson());
        }
        data["articles"].append(article);
    }
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
    Mapper<Tag> mpTag(db);
    Mapper<Article> mpArticle(db);
    Mapper<ArticleTag> mpArticleTag(db);

    int page = req->getAttributes()->get<int>("page");
    int pageSize = req->getAttributes()->get<int>("pageSize");

    auto tag = mpTag.findOne(Criteria(Tag::Cols::_slug, slug));
    auto tagId = tag.getValueOfId();
    data["tag"] = tag.toJson();
    data["total"] = mpArticleTag.count(Criteria(ArticleTag::Cols::_tag, tagId));
    auto res = db->execSqlSync(
        "SELECT "
        "art.id,art.title,art.create_time,art.excerpt,"
        "art.author as author_id,user.realname as author_name,"
        "art.category as cat_id,cat.slug as cat_slug,cat.name as cat_name "
        "FROM article art "
        "JOIN article_tag ON art.id = article_tag.article "
        "JOIN user ON art.author = user.id "
        "JOIN category cat ON art.category = cat.id "
        "WHERE article_tag.tag = ? "
        "ORDER BY art.create_time DESC LIMIT ?,?;",
        tagId, (page-1)*pageSize, pageSize
    );
    for (const auto &row : res) {
        Json::Value art;
        auto artId = row["id"].as<int>();
        art["id"] = artId;
        art["title"] = row["title"].as<std::string>();
        art["create_time"] = row["create_time"].as<std::string>();
        art["excerpt"] = row["excerpt"].as<std::string>();
        art["author"]["id"] = row["author_id"].as<int>();
        art["author"]["realname"] = row["author_name"].as<std::string>();
        art["category"]["id"] = row["cat_id"].as<int>();
        art["category"]["name"] = row["cat_name"].as<std::string>();
        art["category"]["slug"] = row["cat_slug"].as<std::string>();
        auto tagsRes = db->execSqlSync(
            "SELECT tag.id,tag.slug,tag.name FROM tag "
            "JOIN article_tag ON tag.id = article_tag.tag "
            "WHERE article_tag.article = ?;", artId
        );
        for (const auto &tagRow : tagsRes) {
            Json::Value tagItem;
            tagItem["id"] = tagRow["id"].as<int>();
            tagItem["name"] = tagRow["name"].as<std::string>();
            tagItem["slug"] = tagRow["slug"].as<std::string>();
            art["tags"].append(tagItem);
        }
        data["articles"].append(art);
    }
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
