#pragma once

#include <vector>
#include <drogon/drogon.h>
#include "../dtos/Article.h"

using namespace drogon;
using namespace orm;

class ArticleMapper {
public:
    static std::vector<Article> selectLimit(int page, int pageSize) {
        std::vector<Article> articles;
        auto db = drogon::app().getDbClient();
        auto res = db->execSqlSync(selectLimitSql, (page-1)*pageSize, pageSize);
        articles = resultToArticles(res);
        return articles;
    }

    static std::vector<Article> selectLimitByUser(int id, int page, int pageSize) {
        std::vector<Article> articles;
        auto db = drogon::app().getDbClient();
        auto res = db->execSqlSync(selectLimitByUserSql,
            id, (page-1)*pageSize, pageSize);
        articles = resultToArticles(res);
        return articles;
    }

    static std::vector<Article> selectLimitByCategory(
        const std::string &slug, int page, int pageSize
    ) {
        std::vector<Article> articles;
        auto db = drogon::app().getDbClient();
        auto res = db->execSqlSync(selectLimitByCategorySql,
            slug, (page-1)*pageSize, pageSize);
        articles = resultToArticles(res);
        return articles;
    }

    static std::vector<Article> selectLimitByTag(
        const std::string &slug, int page, int pageSize
    ) {
        std::vector<Article> articles;
        auto db = drogon::app().getDbClient();
        auto res = db->execSqlSync(selectLimitByTagSql,
            slug, (page-1)*pageSize, pageSize);
        articles = resultToArticles(res);
        return articles;
    }

    static int count() {
        auto db = app().getDbClient();
        auto res = db->execSqlSync("SELECT COUNT(*) FROM article");
        return res[0][0].as<int>();
    }

    static int countByUser(int id) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync("SELECT COUNT(*) FROM article WHERE author_id = ?",
            id);
        return res[0][0].as<int>();
    }

    static int countByCategory(const std::string &slug) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "SELECT COUNT(*) FROM article a JOIN category c ON a.category_id = c.id "
            "WHERE c.slug = ?",
            slug);
        return res[0][0].as<int>();
    }

    static int countByTag(const std::string &slug) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "SELECT COUNT(*) FROM article a JOIN article_tag at ON a.id = at.article "
            "JOIN tag t ON at.tag = t.id WHERE t.slug = ?",
            slug);
        return res[0][0].as<int>();
    }

    static ArticlePtr select(int id) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync(selectSql, id);
        return resultToArticle(res);
    }

    static void insert(Article &article) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "INSERT INTO article "
            "(title, author_id, category_id, content, excerpt, create_time, modify_time) "
            "VALUE (?, ?, ?, ?, ?, NOW(), NOW())",
            article.title, article.authorId, article.categoryId, article.content,
            article.excerpt
        );
        article.id = res.insertId();
    }

    static void update(Article &article) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "UPDATE article SET modify_time = NOW() "
            + (article.authorId > 0 ? ",author_id = '" + std::to_string(article.authorId) + "'" : "")
            + (article.categoryId > 0 ? ",category_id = '" + std::to_string(article.categoryId) + "'" : "")
            + ",title = '" + article.title + "'"
            + ",excerpt = '" + article.excerpt + "'"
            + ",content = '" + article.content + "'"
            + " WHERE id = '" + std::to_string(article.id) + "'"
        );
    }

    static void updateCategoryToDefault(const std::vector<int> &ids) {
        if (ids.empty()) return;
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "UPDATE article SET category_id = ? WHERE category_id IN "
                + join(ids, "(", ")", ","),
            Category::UNCATEGORIED
        );
    }

    static void deletes(const std::vector<int> &ids, int userId = -1) {
        if (ids.empty()) return;
        auto db = app().getDbClient();
        std::string sql = "DELETE FROM article WHERE id IN "
            + join(ids, "(", ")", ",");
        if (userId > 0) {
            sql += " AND author_id = " + std::to_string(userId);
        }
        auto res = db->execSqlSync(sql);
    }

    static void deleteByUsers(const std::vector<int> &ids) {
        if (ids.empty()) return;
        auto db = app().getDbClient();
        auto res = db->execSqlSync("DELETE FROM article WHERE author_id IN "
            + join(ids, "(", ")", ","));
    }
private:
    static const std::string commonSelectFields;
    static const std::string selectLimitSql;
    static const std::string selectLimitByUserSql;
    static const std::string selectLimitByCategorySql;
    static const std::string selectLimitByTagSql;
    static const std::string selectSql;

    static std::vector<Article> resultToArticles(const Result &result);
    static ArticlePtr resultToArticle(const Result &result);
};