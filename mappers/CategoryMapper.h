#pragma once

#include <vector>
#include <drogon/drogon.h>
#include "../utils/Utils.h"
#include "../dtos/Category.h"

using namespace drogon;

class CategoryMapper {
public:
    static std::vector<Category> selectAll();

    static CategoryPtr select(int id);

    static CategoryPtr selectBySlug(const std::string &slug);

    static void insert(Category &category);

    static void update(Category &category);

    static void deletes(const std::vector<int> ids);
};