#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>
#include "BlogController.h"
#include "../utils/Utils.h"

using orm::Mapper;
using orm::Criteria;
using orm::CompareOperator;
using orm::SortOrder;
using drogon_model::dg_test::Article;
using drogon_model::dg_test::User;
using drogon_model::dg_test::Category;
using drogon_model::dg_test::Tag;
using drogon_model::dg_test::ArticleTag;


void BlogController::articleList(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int page
) const {
    Json::Value json;
    auto db = drogon::app().getDbClient();
    Mapper<Article> mpArticle(db);
    size_t perPage = 10;
    try {
        auto numArticles = mpArticle.count();
        auto articles = mpArticle
            .orderBy(Article::Cols::_create_time, orm::SortOrder::DESC)
            .paginate(page, perPage).findAll();
        json["num_pages"] = numArticles / perPage + (numArticles%perPage?1:0);
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
            article["create_time"] = art.getValueOfCreateTime().toDbString();
            article["excerpt"] = art.getValueOfExcerpt();
            json["articles"].append(article);
        }
        json["status"] = 0;
    } catch (const orm::DrogonDbException &ex) {
        json["status"] = 2;
    }

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void BlogController::articleListAdmin(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int page
) const {
    auto token = req->getHeader("Authorization").substr(7);
    auto decoded = jwt::decode<json_traits>(token);
    int userId = decoded.get_payload_claim("uid").as_integer();

    Json::Value json;
    auto db = drogon::app().getDbClient();
    Mapper<User> mpUser(db);
    Mapper<Article> mpArticle(db);
    size_t perPage = 10;
    try {
        auto userInDb = mpUser.findOne(Criteria(User::Cols::_id, userId));
        auto roleId = userInDb.getValueOfRole();
        std::vector<Article> articles;
        int numArticles = 0;
        if (roleId <= 2) {
            numArticles = mpArticle.count();
            articles = mpArticle.orderBy(Article::Cols::_create_time,
                orm::SortOrder::DESC).paginate(page, perPage).findAll();
        } else if (roleId == 3) {
            numArticles = mpArticle.count(Criteria(Article::Cols::_author, userId));
            articles = mpArticle
                .orderBy(Article::Cols::_create_time,SortOrder::DESC)
                .paginate(page, perPage)
                .findBy(Criteria(Article::Cols::_author, userId));
        }
        json["num_pages"] = numArticles / perPage + (numArticles%perPage?1:0);
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
                .toCustomedFormattedString("%Y-%m-%d");
            article["excerpt"] = art.getValueOfExcerpt();
            json["articles"].append(article);
        }
        json["status"] = 0;
    } catch (const orm::DrogonDbException &ex) {
        LOG_DEBUG << ex.base().what();
        json["status"] = 2;
    }

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void BlogController::getArticle(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int id
) const {
    Json::Value json;
    auto db = app().getDbClient();
    Mapper<Article> mpArticle(db);
    int roleId = 5;
    try {
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
        article["create_time"] = art.getValueOfCreateTime().toDbString();
        article["content"] = art.getValueOfContent();
        article["excerpt"] = art.getValueOfExcerpt();
        json["article"] = article;
        json["status"] = 0;
    } catch (const orm::DrogonDbException &ex) {
        json["status"] = 2;
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
    }
    
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void BlogController::updateArticle(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const drogon_model::dg_test::Article &article,
    const std::vector<std::string> &tags
) const {
    bool hasPermission = false;
    Json::Value json;
    auto token = req->getHeader("Authorization").substr(7);
    auto decoded = jwt::decode<json_traits>(token);
    int userId = decoded.get_payload_claim("uid").as_integer();

    auto db = app().getDbClient();
    Mapper<Article> mpArticle(db);
    Mapper<Tag> mpTag(db);
    Mapper<ArticleTag> mpArticleTag(db);
    Mapper<User> mpUser(db);
    
    try {
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
            json["status"] = 0;
        } else {
            json["status"] = 4;
            json["error"] = "没有权限";
        }
    }
    catch (const std::exception &ex) {
        json["status"] = 2;
    }

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void BlogController::deleteArticles(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    Json::Value json;

    auto token = req->getHeader("Authorization").substr(7);
    auto decoded = jwt::decode<json_traits>(token);
    int userId = decoded.get_payload_claim("uid").as_integer();

    auto pJson = req->getJsonObject();
    if (!pJson) {
        throw std::invalid_argument("请求体格式错误, 请使用json");
    }
    const auto &reqJson = *pJson;
    if (!reqJson.isMember("ids") || reqJson["ids"].type() != Json::arrayValue) {
        throw std::invalid_argument("缺少必备字段: ids, 或者类型错误");
    }

    int count = reqJson["ids"].size();
    if (count <= 0) {
        json["status"] = 2;
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }

    Json::Value ids = reqJson["ids"];
    std::vector<int> idList(count);
    for (int i=0; i<count; ++i) {
        idList[i] = ids[i].asUInt();
    }
    Mapper<Article> mp(app().getDbClient());
    try {
        mp.deleteBy(
            Criteria(Article::Cols::_id, CompareOperator::In, idList)
        );
        json["status"] = 0;
    } catch (const orm::DrogonDbException &ex) {
        LOG_DEBUG << ex.base().what();
        json["status"] = 2;
    }
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void BlogController::getCategories(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    Json::Value json;
    Mapper<Category> mp(app().getDbClient());
    try {
        auto categoires = mp.findAll();
        for (const auto &cat : categoires) {
            Json::Value item;
            item["name"] = cat.getValueOfName();
            item["id"] = cat.getValueOfId();
            item["slug"] = cat.getValueOfSlug();
            json["categories"].append(item);
        }
        json["status"] = 0;
    } catch (const orm::DrogonDbException &ex) {
        json["status"] = 2;
    }
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void BlogController::articleListByCategory(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const std::string &slug,
    int page
) const {
    Json::Value json;
    size_t perPage = 10;
    auto db = app().getDbClient();
    Mapper<Category> mpCategory(db);
    Mapper<Article> mpArticle(db);
    try {
        auto cat = mpCategory.findOne(Criteria(Category::Cols::_slug, slug));
        json["category"] = cat.toJson();
        auto numArticles = mpArticle.count(
            Criteria(Article::Cols::_category, cat.getValueOfId()));
        auto num_pages = numArticles / perPage + (numArticles%perPage?1:0);
        json["num_pages"] = num_pages;
        auto articles = mpArticle
            .orderBy(Article::Cols::_create_time, SortOrder::DESC)
            .paginate(page, perPage)
            .findBy(Criteria(Article::Cols::_category, cat.getValueOfId()));
        for (const auto &art : articles) {
            Json::Value article;
            article["id"] = art.getValueOfId();
            article["title"] = art.getValueOfTitle();
            article["create_time"] = art.getValueOfCreateTime().toDbString();
            article["excerpt"] = art.getValueOfExcerpt();
            auto author = art.getUser(db);
            article["author"]["id"] = author.getValueOfId();
            article["author"]["realname"] = author.getValueOfRealname();
            auto tags = art.getTags(db);
            for (const auto &tag : tags) {
                article["tags"].append(tag.first.toJson());
            }
            json["articles"].append(article);
        }
        json["status"] = 0;
    } catch (const orm::DrogonDbException &ex) {
        json["status"] = 2;
    }
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void BlogController::articleListByAuthor(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int id,
    int page
) const {
    Json::Value json;
    size_t perPage = 10;
    auto db = app().getDbClient();
    Mapper<User> mpUser(db);
    Mapper<Article> mpArticle(db);
    try {
        auto author = mpUser.findOne(Criteria(User::Cols::_id, id));
        json["author"]["id"] = author.getValueOfId();
        json["author"]["realname"] = author.getValueOfRealname();
        auto numArticles = mpArticle.count(
            Criteria(Article::Cols::_author, author.getValueOfId()));
        auto numPages = numArticles / perPage + (numArticles%perPage?1:0);
        json["num_pages"] = numPages;
        auto articles = mpArticle.orderBy(Article::Cols::_create_time, SortOrder::DESC)
            .paginate(page, perPage)
            .findBy(Criteria(Article::Cols::_author, author.getValueOfId()));
        for (const auto &art : articles) {
            Json::Value article;
            article["id"] = art.getValueOfId();
            article["title"] = art.getValueOfTitle();
            article["create_time"] = art.getValueOfCreateTime().toDbString();
            article["excerpt"] = art.getValueOfExcerpt();
            article["category"] = art.getCategory(db).toJson();
            auto tags = art.getTags(db);
            for (const auto &tag : tags) {
                article["tags"].append(tag.first.toJson());
            }
            json["articles"].append(article);
        }
        json["status"] = 0;
    } catch (const orm::DrogonDbException &ex) {
        json["status"] = 2;
    }
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void BlogController::articleListByTag(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    const std::string &slug,
    int page
) const {
    Json::Value json;
    size_t perPage = 10;
    auto db = app().getDbClient();
    Mapper<Tag> mpTag(db);
    Mapper<Article> mpArticle(db);
    Mapper<ArticleTag> mpArticleTag(db);
    try {
        auto tag = mpTag.findOne(Criteria(Tag::Cols::_slug, slug));
        auto tagId = tag.getValueOfId();
        json["tag"] = tag.toJson();
        auto numArticles = mpArticleTag.count(Criteria(ArticleTag::Cols::_tag, tagId));
        auto numPages = numArticles / perPage + (numArticles%perPage?1:0);
        json["num_pages"] = numPages;
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
            tagId, (page-1)*perPage, perPage
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
            json["articles"].append(art);
        }
        json["status"] = 0;
    } catch (const orm::DrogonDbException &ex) {
        json["status"] = 2;
    }
    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}