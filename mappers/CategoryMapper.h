#pragma once

#include <vector>
#include <drogon/drogon.h>
#include "../dtos/Category.h"

using namespace drogon;

class CategoryMapper {
public:
    static std::vector<Category> selectAll() {
        std::vector<Category> categories;
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "SELECT id,slug,name,create_time,modify_time FROM category");
        for (const auto &row : res) {
            Category c;
            c.id = row["id"].as<int>();
            c.slug = row["slug"].as<std::string>();
            c.name = row["name"].as<std::string>();
            c.createTime = row["create_time"].as<std::string>();
            c.modifyTime = row["modify_time"].as<std::string>();
            categories.emplace_back(c);
        }
        return categories;
    }

    static CategoryPtr select(int id) {
        CategoryPtr c;
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "SELECT id,slug,name,create_time,modify_time FROM category WHERE id = ?", id);
        if (!res.empty()) {
            auto row = res[0];
            c = std::make_shared<Category>();
            c->id = row["id"].as<int>();
            c->slug = row["slug"].as<std::string>();
            c->name = row["name"].as<std::string>();
            c->createTime = row["create_time"].as<std::string>();
            c->modifyTime = row["modify_time"].as<std::string>();
        }
        return c;
    }
};