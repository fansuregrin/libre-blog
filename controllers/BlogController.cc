#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>
#include "BlogController.h"
#include "Utils.h"

using orm::Mapper;
using drogon_model::dg_test::Article;
using drogon_model::dg_test::User;
using drogon_model::dg_test::Category;
using json_traits = jwt::traits::nlohmann_json;

void BlogController::articleList(
    const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback,
    int page
) const {
    auto db = drogon::app().getDbClient();
    Mapper<Article> mp(db);
    size_t per_page = 10;
    auto num_articles = mp.count();
    auto num_pages = num_articles / per_page + (num_articles%per_page?1:0);
    auto articles = mp.orderBy(Article::Cols::_create_time, orm::SortOrder::DESC)
        .paginate(page, per_page).findAll();
    Json::Value json;
    if (!articles.empty()) {
        json["status"] = 0;
        json["num_pages"] = num_pages;
    } else {
        json["status"] = 1;
    }
    for (const auto &art : articles) {
        Json::Value article;
        article["id"] = art.getValueOfId();
        article["title"] = art.getValueOfTitle();
        article["author"] = *art.getAuthor();
        article["author_name"] = art.getUser(db).getValueOfRealname();
        article["category"] = *art.getCategory();
        article["category_name"] = art.getCategory(db).getValueOfName();
        article["create_time"] = art.getValueOfCreateTime()
            .toCustomedFormattedString("%Y-%m-%d");
        article["excerpt"] = art.getValueOfExcerpt();
        json["articles"].append(article);
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
    auto tmp = req->getHeader("Authorization");
    if (!tmp.empty() && tmp.compare(0, 7, "Bearer ") == 0) {
        token = tmp.substr(7);
    }
    int userId = -1;
    if (!token.empty()) {
        try {
            auto decoded = jwt::decode<json_traits>(token);
            jwt::verify<json_traits>()
                .allow_algorithm(jwt::algorithm::es256k(es256k_pub_key, es256k_priv_key))
                .with_issuer("drogon")
                .verify(decoded);
            userId = decoded.get_payload_claim("uid").as_integer();
        } catch (const std::exception &ex) {
            LOG_INFO << ex.what();
        }
    }

    auto db = app().getDbClient();
    Mapper<Article> mp(db);
    
    Json::Value json;
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
    if (!token.empty()) {
        try {
            auto decoded = jwt::decode<json_traits>(token);
            jwt::verify<json_traits>()
                .allow_algorithm(jwt::algorithm::es256k(es256k_pub_key, es256k_priv_key))
                .with_issuer("drogon")
                .verify(decoded);
            userId = decoded.get_payload_claim("uid").as_integer();
        } catch (const std::exception &ex) {
            LOG_INFO << ex.what();
        }
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
                {Article::Cols::_title, Article::Cols::_excerpt, Article::Cols::_content},
                {Article::Cols::_id, article.getValueOfId()},
                article.getValueOfTitle(), article.getValueOfExcerpt(),
                article.getValueOfContent()
            );
            json["status"] = 0;
        } catch (const std::exception &ex) {
            json["status"] = 2;
        }
    } else {
        json["status"] = 2;
    }

    auto resp = HttpResponse::newHttpJsonResponse(json);
    callback(resp);
}