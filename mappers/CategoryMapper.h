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
            const auto &row = res[0];
            c = std::make_shared<Category>();
            c->id = row["id"].as<int>();
            c->slug = row["slug"].as<std::string>();
            c->name = row["name"].as<std::string>();
            c->createTime = row["create_time"].as<std::string>();
            c->modifyTime = row["modify_time"].as<std::string>();
        }
        return c;
    }

    static CategoryPtr selectBySlug(const std::string &slug) {
        CategoryPtr c;
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "SELECT id,slug,name,create_time,modify_time FROM category WHERE slug = ?",
            slug);
        if (!res.empty()) {
            const auto &row = res[0];
            c = std::make_shared<Category>();
            c->id = row["id"].as<int>();
            c->slug = row["slug"].as<std::string>();
            c->name = row["name"].as<std::string>();
            c->createTime = row["create_time"].as<std::string>();
            c->modifyTime = row["modify_time"].as<std::string>();
        }
        return c;
    }

    static void insert(Category &category) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "INSERT INTO category (slug,name,create_time,modify_time) VALUE "
            "(?, ?, NOW(), NOW())",
            category.slug, category.name);
        category.id = res.insertId();
    }

    static void update(Category &category) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "UPDATE category SET modify_time = NOW() "
            + (category.slug.empty() ? "" : ",slug = " + category.slug)
            + (category.name.empty() ? "" : ",name = " + category.name)
            + " WHERE id = ?",
            category.id
        );
    }

    static void deletes(const std::vector<int> ids) {
        if (ids.empty()) return;
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "DELTE FROM category WHERE id IN ?",
            join(ids, "(", ")", ",")
        );
    }
};