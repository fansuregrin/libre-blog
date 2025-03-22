#include "ArticleMapper.h"

const std::string ArticleMapper::commonSelectFields =
R"(SELECT a.id, a.title, a.excerpt, a.create_time, a.modify_time,
u.id AS user_id, u.username AS user_username, u.realname AS user_realname,
c.id AS category_id, c.slug AS category_slug, c.name AS category_name,
t.id AS tag_id, t.slug AS tag_slug, t.name AS tag_name )";

const std::string ArticleMapper::selectLimitSql = ArticleMapper::commonSelectFields +
R"(FROM (SELECT id FROM article ORDER BY modify_time DESC LIMIT ?,?) sub
JOIN article a ON sub.id = a.id
LEFT JOIN category c ON a.category_id = c.id
LEFT JOIN user u ON a.author_id = u.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id;)";

const std::string ArticleMapper::selectLimitByUserSql = 
ArticleMapper::commonSelectFields +
R"(FROM (SELECT id FROM article WHERE author_id = ? ORDER BY modify_time DESC LIMIT ?,?) sub
JOIN article a ON sub.id = a.id
LEFT JOIN category c ON a.category_id = c.id
JOIN user u ON a.author_id = u.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id;)";

const std::string ArticleMapper::selectLimitByCategorySql = 
ArticleMapper::commonSelectFields +
R"(FROM (SELECT a1.id FROM article a1 LEFT JOIN category c1 ON a1.category_id = c1.id
    WHERE c1.slug = ? ORDER BY a1.modify_time DESC LIMIT ?,?) sub
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
    ORDER BY a1.modify_time DESC LIMIT ?,?) sub
JOIN article a ON sub.id = a.id
LEFT JOIN category c ON a.category_id = c.id
LEFT JOIN user u ON a.author_id = u.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id;)";

const std::string ArticleMapper::selectSql = 
ArticleMapper::commonSelectFields +
R"(,a.content FROM article a
LEFT JOIN user u ON a.author_id = u.id
LEFT JOIN category c ON a.category_id = c.id
LEFT JOIN article_tag at ON a.id = at.article
LEFT JOIN tag t ON at.tag = t.id
WHERE a.id = ?;)";

std::vector<Article> ArticleMapper::selectLimit(int page, int pageSize) {
    std::vector<Article> articles;
    auto db = drogon::app().getDbClient();
    auto res = db->execSqlSync(selectLimitSql, (page-1)*pageSize, pageSize);
    articles = resultToArticles(res);
    return articles;
}

std::vector<Article> ArticleMapper::selectLimitByUser(int id, int page, int pageSize) {
    std::vector<Article> articles;
    auto db = drogon::app().getDbClient();
    auto res = db->execSqlSync(selectLimitByUserSql,
        id, (page-1)*pageSize, pageSize);
    articles = resultToArticles(res);
    return articles;
}

std::vector<Article> ArticleMapper::selectLimitByCategory(
    const std::string &slug, int page, int pageSize
) {
    std::vector<Article> articles;
    auto db = drogon::app().getDbClient();
    auto res = db->execSqlSync(selectLimitByCategorySql,
        slug, (page-1)*pageSize, pageSize);
    articles = resultToArticles(res);
    return articles;
}

std::vector<Article> ArticleMapper::selectLimitByTag(
    const std::string &slug, int page, int pageSize
) {
    std::vector<Article> articles;
    auto db = drogon::app().getDbClient();
    auto res = db->execSqlSync(selectLimitByTagSql,
        slug, (page-1)*pageSize, pageSize);
    articles = resultToArticles(res);
    return articles;
}

int ArticleMapper::count() {
    auto db = app().getDbClient();
    auto res = db->execSqlSync("SELECT COUNT(*) FROM article");
    return res[0][0].as<int>();
}

int ArticleMapper::countByUser(int id) {
    auto db = app().getDbClient();
    auto res = db->execSqlSync("SELECT COUNT(*) FROM article WHERE author_id = ?",
        id);
    return res[0][0].as<int>();
}

int ArticleMapper::countByCategory(const std::string &slug) {
    auto db = app().getDbClient();
    auto res = db->execSqlSync(
        "SELECT COUNT(*) FROM article a JOIN category c ON a.category_id = c.id "
        "WHERE c.slug = ?",
        slug);
    return res[0][0].as<int>();
}

int ArticleMapper::countByTag(const std::string &slug) {
    auto db = app().getDbClient();
    auto res = db->execSqlSync(
        "SELECT COUNT(*) FROM article a JOIN article_tag at ON a.id = at.article "
        "JOIN tag t ON at.tag = t.id WHERE t.slug = ?",
        slug);
    return res[0][0].as<int>();
}

ArticlePtr ArticleMapper::select(int id) {
    auto db = app().getDbClient();
    auto res = db->execSqlSync(selectSql, id);
    return resultToArticle(res);
}

void ArticleMapper::insert(Article &article) {
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

void ArticleMapper::update(Article &article) {
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

void ArticleMapper::updateCategoryToDefault(const std::vector<int> &ids) {
    if (ids.empty()) return;
    auto db = app().getDbClient();
    auto res = db->execSqlSync(
        "UPDATE article SET category_id = ? WHERE category_id IN "
            + join(ids, "(", ")", ","),
        Category::UNCATEGORIED
    );
}

void ArticleMapper::deletes(const std::vector<int> &ids, int userId) {
    if (ids.empty()) return;
    auto db = app().getDbClient();
    std::string sql = "DELETE FROM article WHERE id IN "
        + join(ids, "(", ")", ",");
    if (userId > 0) {
        sql += " AND author_id = " + std::to_string(userId);
    }
    auto res = db->execSqlSync(sql);
}

void ArticleMapper::deleteByUsers(const std::vector<int> &ids) {
    if (ids.empty()) return;
    auto db = app().getDbClient();
    auto res = db->execSqlSync("DELETE FROM article WHERE author_id IN "
        + join(ids, "(", ")", ","));
}

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
            a.modifyTime = row["modify_time"].as<std::string>();
            a.author.id = row["user_id"].as<int>();
            a.author.username = row["user_username"].as<std::string>();
            a.author.realname = row["user_realname"].as<std::string>();
            a.category.id = row["category_id"].as<int>();
            a.category.slug = row["category_slug"].as<std::string>();
            a.category.name = row["category_name"].as<std::string>();
            a.authorId = a.author.id;
            a.categoryId = a.category.id;
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
            a->modifyTime = row["modify_time"].as<std::string>();
            a->author.id = row["user_id"].as<int>();
            a->author.username = row["user_username"].as<std::string>();
            a->author.realname = row["user_realname"].as<std::string>();
            a->category.id = row["category_id"].as<int>();
            a->category.slug = row["category_slug"].as<std::string>();
            a->category.name = row["category_name"].as<std::string>();
            a->authorId = a->author.id;
            a->categoryId = a->category.id;
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