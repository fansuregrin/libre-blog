#pragma once

#include <drogon/drogon.h>
#include <vector>
#include "../dtos/Tag.h"
#include "../utils/Utils.h"

using namespace drogon;

class TagMapper {
public:
    static std::vector<Tag> selectAll();

    static int count();

    static std::vector<Tag> selectLimit(int page, int pageSize);

    static TagPtr selectById(int id);

    static TagPtr selectBySlug(const std::string &slug);

    static TagPtr selectByName(const std::string &name);

    static void insert(Tag &tag);

    static void update(Tag &tag);

    static void deletes(std::vector<int> ids);
};