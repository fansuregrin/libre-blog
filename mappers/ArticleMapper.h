#pragma once

#include <vector>
#include <drogon/drogon.h>
#include "../dtos/Article.h"

using namespace drogon;
using namespace orm;

class ArticleMapper {
public:
    static std::vector<Article> selectLimit(int page, int pageSize);

    static std::vector<Article> selectLimitByUser(int id, int page, int pageSize);

    static std::vector<Article> selectLimitByCategory(
        const std::string &slug, int page, int pageSize
    );

    static std::vector<Article> selectLimitByTag(
        const std::string &slug, int page, int pageSize
    );

    static int count();

    static int countByUser(int id);
    
    static int countByCategory(const std::string &slug);

    static int countByTag(const std::string &slug);

    static ArticlePtr select(int id);

    static void insert(Article &article);

    static void update(Article &article);

    static void updateCategoryToDefault(const std::vector<int> &ids);

    static void deletes(const std::vector<int> &ids, int userId = -1);

    static void deleteByUsers(const std::vector<int> &ids);
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