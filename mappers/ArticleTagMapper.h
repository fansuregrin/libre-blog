#pragma once

#include <drogon/drogon.h>
#include "../dtos/ArticleTag.h"

using namespace drogon;

class ArticleTagMapper {
public:
    static void insert(ArticleTag &at) {
        auto db = app().getDbClient();
        db->execSqlSync("INSERT INTO article_tag (article,tag) VALUE (?,?)",
            at.article, at.tag);
    }

    static void deleteByArticle(int id) {
        auto db = app().getDbClient();
        db->execSqlSync("DELETE FROM article_tag WHERE article = ?", id);
    }

    static void deleteByArticles(const std::vector<int> &ids, int userId = -1) {
        if (ids.empty()) return;
        auto db = app().getDbClient();
        std::string sql;
        if (userId > 0) {
            sql = "DELETE FROM article_tag at JOIN article a ON at.article = a.id "
            " AND a.author_id = " + std::to_string(userId) +
            " WHERE at.article IN " + join(ids, "(", ")", ",");
        } else {
            sql = "DELETE FROM article_tag WHERE article IN "
                + join(ids, "(", ")", ",");
        }
        db->execSqlSync(sql);
    }

    static void deleteByUsers(const std::vector<int> &ids) {
        if (ids.empty()) return;
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "DELETE FROM article_tag at JOIN article a ON at.article = a.id "
            "AND a.author_id IN " + join(ids, "(", ")", ",")
        );
    }

    static void deleteByTag(int id) {
        auto db = app().getDbClient();
        db->execSqlSync("DELETE FROM article_tag WHERE tag = ?", id);
    }

    static void deleteByTags(const std::vector<int> &ids) {
        if (ids.empty()) return;
        auto db = app().getDbClient();
        db->execSqlSync("DELETE FROM article_tag WHERE tag IN " 
            + join(ids, "(", ")", ","));
    }
};