#include "ArticleTagMapper.h"

void ArticleTagMapper::insert(ArticleTag &at) {
    auto db = app().getDbClient();
    db->execSqlSync("INSERT INTO article_tag (article,tag) VALUE (?,?)",
        at.article, at.tag);
}

void ArticleTagMapper::deleteByArticle(int id) {
    auto db = app().getDbClient();
    db->execSqlSync("DELETE FROM article_tag WHERE article = ?", id);
}

void ArticleTagMapper::deleteByArticles(const std::vector<int> &ids, int userId) {
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

void ArticleTagMapper::deleteByUsers(const std::vector<int> &ids) {
    if (ids.empty()) return;
    auto db = app().getDbClient();
    auto res = db->execSqlSync(
        "DELETE FROM article_tag at JOIN article a ON at.article = a.id "
        "AND a.author_id IN " + join(ids, "(", ")", ",")
    );
}

void ArticleTagMapper::deleteByTag(int id) {
    auto db = app().getDbClient();
    db->execSqlSync("DELETE FROM article_tag WHERE tag = ?", id);
}

void ArticleTagMapper::deleteByTags(const std::vector<int> &ids) {
    if (ids.empty()) return;
    auto db = app().getDbClient();
    db->execSqlSync("DELETE FROM article_tag WHERE tag IN " 
        + join(ids, "(", ")", ","));
}