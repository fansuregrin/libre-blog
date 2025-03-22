#pragma once

#include <drogon/drogon.h>
#include "../utils/Utils.h"
#include "../dtos/ArticleTag.h"

using namespace drogon;

class ArticleTagMapper {
public:
    static void insert(ArticleTag &at);

    static void deleteByArticle(int id);

    static void deleteByArticles(const std::vector<int> &ids, int userId = -1);

    static void deleteByUsers(const std::vector<int> &ids);

    static void deleteByTag(int id);

    static void deleteByTags(const std::vector<int> &ids);
};