#include "ArticleMapper.h"

const std::string ArticleMapper::commonSelectFields =
R"(SELECT a.id, a.title, a.excerpt, a.create_time,
u.id AS user_id, u.username AS user_username, u.realname AS user_realname,
c.id AS category_id, c.slug AS category_slug, c.name AS category_name,
t.id AS tag_id, t.slug AS tag_slug, t.name AS tag_name )";

const std::string ArticleMapper::selectLimitSql = ArticleMapper::commonSelectFields +
R"(FROM (SELECT id FROM article ORDER BY create_time DESC LIMIT ?,?) sub
JOIN article a ON sub.id = a.id
LEFT JOIN category c ON a.category_id = c.id
LEFT JOIN user u ON a.author_id = u.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id;)";

const std::string ArticleMapper::selectLimitByUserSql = 
ArticleMapper::commonSelectFields +
R"(FROM (SELECT id FROM article WHERE author = ? ORDER BY create_time DESC LIMIT ?,?) sub
JOIN article a ON sub.id = a.id
LEFT JOIN category c ON a.category_id = c.id
JOIN user u ON a.author_id = u.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id;)";

const std::string ArticleMapper::selectLimitByCategorySql = 
ArticleMapper::commonSelectFields +
R"(FROM (SELECT a1.id FROM article a1 LEFT JOIN category c1 ON a1.category = c1.id
    WHERE c1.slug = ? ORDER BY a1.create_time DESC LIMIT ?,?) sub
JOIN article a ON sub.id = a.id
JOIN category c ON a.category_id = c.id
LEFT JOIN user u ON a.author_id = u.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id;)";

const std::string ArticleMapper::selectLimitByTagSql = 
ArticleMapper::commonSelectFields +
R"(FROM (SELECT a1.id FROM article a1
    JOIN article_tag at1 ON a1.id = at1.article
    JOIN tag t1 ON at1.tag = t1.id WHERE t1.slug = ?
    ORDER BY a1.create_time DESC LIMIT ?,?) sub
JOIN article a ON sub.id = a.id
LEFT JOIN category c ON a.category_id = c.id
LEFT JOIN user u ON a.author_id = u.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id;)";

const std::string ArticleMapper::selectSql = 
ArticleMapper::commonSelectFields +
R"(FROM article a
LEFT JOIN user u ON a.author_id = u.id
LEFT JOIN category c ON a.category_id = c.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id
WHERE a.id = ?;)";

std::vector<Article> ArticleMapper::resultToArticles(const Result &result) {
    std::vector<Article> articles;
    std::unordered_map<int, Article> articlesMap;
    std::vector<int> ids;
    for (const auto &row : result) {
        int id = row["id"].as<int>();
        if (articlesMap.find(id) == articlesMap.end()) {
            ids.emplace_back(id);
            Article a;
            a.id = id;
            a.title = row["title"].as<std::string>();
            a.excerpt = row["excerpt"].as<std::string>();
            a.createTime = row["create_time"].as<std::string>();
            a.author.id = row["user_id"].as<int>();
            a.author.username = row["user_username"].as<std::string>();
            a.author.realname = row["user_realname"].as<std::string>();
            a.category.id = row["category_id"].as<int>();
            a.category.slug = row["category_slug"].as<std::string>();
            a.category.name = row["category_name"].as<std::string>();
            articlesMap[id] = a;
        }
        if (!row["tag_id"].isNull()) {
            Tag tag;
            tag.id = row["tag_id"].as<int>();
            tag.slug = row["tag_slug"].as<std::string>();
            tag.name = row["tag_name"].as<std::string>();
            articlesMap[id].tags.emplace_back(tag);
        }
    }
    for (int i : ids) {
        articles.emplace_back(articlesMap[i]);
    }
    return articles;
}

ArticlePtr ArticleMapper::resultToArticle(const Result &result) {
    ArticlePtr a;
    bool first = true;
    for (const auto &row : result) {
        if (first) {
            first = false;
            a = std::make_shared<Article>();
            a->id = row["id"].as<int>();
            a->title = row["title"].as<std::string>();
            a->excerpt = row["excerpt"].as<std::string>();
            a->content = row["content"].as<std::string>();
            a->createTime = row["create_time"].as<std::string>();
            a->author.id = row["user_id"].as<int>();
            a->author.username = row["user_username"].as<std::string>();
            a->author.realname = row["user_realname"].as<std::string>();
            a->category.id = row["category_id"].as<int>();
            a->category.slug = row["category_slug"].as<std::string>();
            a->category.name = row["category_name"].as<std::string>();
        }
        if (!row["tag_id"].isNull()) {
            Tag tag;
            tag.id = row["tag_id"].as<int>();
            tag.slug = row["tag_slug"].as<std::string>();
            tag.name = row["tag_name"].as<std::string>();
            a->tags.emplace_back(tag);
        }
    }
    return a;
}