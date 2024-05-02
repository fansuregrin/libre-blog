#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>
#include "BlogController.h"
#include "Utils.h"

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
    Mapper<Article> mp(db);
    size_t per_page = 10;
    try {
        auto num_articles = mp.count();
        auto num_pages = num_articles / per_page + (num_articles%per_page?1:0);
        auto articles = mp.orderBy(Article::Cols::_create_time, orm::SortOrder::DESC)
            .paginate(page, per_page).findAll();
        json["status"] = 0;
        json["num_pages"] = num_pages;
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
    } catch (const orm::DrogonDbException &ex) {
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
    std::string token;
    int userId = -1;
    auto tmp = req->getHeader("Authorization");
    if (!tmp.empty() && tmp.compare(0, 7, "Bearer ") == 0) {
        token = tmp.substr(7);
        verifyUserToken(token, userId);
    }

    Json::Value json;
    auto db = app().getDbClient();
    Mapper<Article> mp(db);
    
    try {
        auto art = mp.findOne({Article::Cols::_id, id});
        int author_id = *art.getAuthor();
        json["status"] = 0;
        Json::Value article;
        article["id"] = art.getValueOfId();
        article["title"] = art.getValueOfTitle();
        article["author"] = author_id;
        article["author_name"] = art.getUser(db).getValueOfRealname();
        article["category"] = *art.getCategory();
        article["category_name"] = art.getCategory(db).getValueOfName();
        auto tags = art.getTags(db);
        for (const auto &tag : tags) {
            article["tags"].append(tag.first.getValueOfName());
        }
        article["create_time"] = art.getValueOfCreateTime()
            .toCustomedFormattedString("%Y-%m-%d");
        article["content"] = art.getValueOfContent();
        article["excerpt"] = art.getValueOfExcerpt();
        article["editable"] = (author_id == userId);
        json["article"] = article;

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
    const drogon_model::dg_test::Article &article
) const {
    bool hasPermission = false;
    Json::Value json;
    
    std::string token;
    auto tmp = req->getHeader("Authorization");
    if (!tmp.empty() && tmp.compare(0, 7, "Bearer ") == 0) {
        token = tmp.substr(7);
    }
    int userId = -1;
    if (!verifyUserToken(token, userId)) {
        json["status"] = 3;
        json["error"] = "登录已失效";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }

    Mapper<Article> mp(app().getDbClient());
    try {
        auto artInDb = mp.findOne({Article::Cols::_id, article.getValueOfId()});
        hasPermission = (userId == *artInDb.getAuthor());
    } catch (const orm::DrogonDbException &ex) {
        LOG_DEBUG << ex.base().what();
    }

    if (hasPermission) {
        try {
            mp.updateBy(
                {Article::Cols::_title, Article::Cols::_category,
                 Article::Cols::_excerpt, Article::Cols::_content},
                {Article::Cols::_id, article.getValueOfId()},
                article.getValueOfTitle(), article.getValueOfCategory(),
                article.getValueOfExcerpt(), article.getValueOfContent()
            );
            json["status"] = 0;
        } catch (const std::exception &ex) {
            json["status"] = 2;
        }
    } else {
        json["status"] = 4;
        json["error"] = "没有权限";
    }

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}

void BlogController::deleteArticles(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback
) const {
    Json::Value json;

    std::string token;
    auto tmp = req->getHeader("Authorization");
    if (!tmp.empty() && tmp.compare(0, 7, "Bearer ") == 0) {
        token = tmp.substr(7);
    }
    int userId = -1;
    if (!verifyUserToken(token, userId)) {
        json["status"] = 3;
        json["error"] = "登录已失效";
        auto resp = HttpResponse::newHttpJsonResponse(json);
        callback(resp);
        return;
    }

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
    std::string slug,
    int page
) const {
    Json::Value json;
    size_t per_page = 10;
    auto db = app().getDbClient();
    Mapper<Category> mpCategory(db);
    Mapper<Article> mpArticle(db);
    try {
        auto cat = mpCategory.findOne(Criteria(Category::Cols::_slug, CompareOperator::EQ, slug));
        auto cat_name = cat.getValueOfName();
        json["name"] = cat_name;
        auto num_articles = mpArticle.count(
            Criteria(Article::Cols::_category, CompareOperator::EQ, cat.getValueOfId()));
        auto num_pages = num_articles / per_page + (num_articles%per_page?1:0);
        json["num_pages"] = num_pages;
        auto articles = mpArticle.orderBy(Article::Cols::_create_time, SortOrder::DESC)
            .paginate(page, per_page)
            .findBy(Criteria(Article::Cols::_category, CompareOperator::EQ, cat.getValueOfId()));
        for (const auto &art : articles) {
            Json::Value article;
            article["id"] = art.getValueOfId();
            article["title"] = art.getValueOfTitle();
            article["author"] = *art.getAuthor();
            article["author_name"] = art.getUser(db).getValueOfRealname();
            article["category"] = *art.getCategory();
            auto tags = art.getTags(db);
            for (const auto &tag : tags) {
                article["tags"].append(tag.first.getValueOfName());
            }
            article["category_name"] = cat_name;
            article["create_time"] = art.getValueOfCreateTime()
                .toCustomedFormattedString("%Y-%m-%d");
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