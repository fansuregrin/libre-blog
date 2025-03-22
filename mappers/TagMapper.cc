#include "TagMapper.h"

std::vector<Tag> TagMapper::selectAll() {
    auto db = app().getDbClient();
    auto result = db->execSqlSync("SELECT id,slug,name FROM tag");
    std::vector<Tag> tags;
    for (const auto &row : result) {
        Tag tag;
        tag.id = row["id"].as<int>();
        tag.name = row["name"].as<std::string>();
        tag.slug = row["slug"].as<std::string>();
        tags.emplace_back(tag);
    }
    return tags;
}

int TagMapper::count() {
    auto db = app().getDbClient();
    return db->execSqlSync("SELECT COUNT(*) FROM tag")[0][0].as<int>();
}

std::vector<Tag> TagMapper::selectLimit(int page, int pageSize) {
    auto db = app().getDbClient();
    auto result = db->execSqlSync(
        "SELECT id,slug,name,create_time,modify_time FROM tag "
        "LIMIT ?,? ORDER BY modify_time DESC",
        (page-1)*pageSize, pageSize);
    std::vector<Tag> tags;
    for (const auto &row : result) {
        Tag tag;
        tag.id = row["id"].as<int>();
        tag.slug = row["slug"].as<std::string>();
        tag.name = row["name"].as<std::string>();
        tag.createTime = row["create_time"].as<std::string>();
        tag.modifyTime = row["modify_time"].as<std::string>();
        tags.emplace_back(tag);
    }
    return tags;
}

TagPtr TagMapper::selectById(int id) {
    auto db = app().getDbClient();
    auto result = db->execSqlSync(
        "SELECT id,slug,name,create_time,modify_time FROM tag WHERE id = ?", id);
    TagPtr tag;
    if (!result.empty()) {
        auto row = result[0];
        tag = std::make_shared<Tag>();
        tag->id = row["id"].as<int>();
        tag->slug  = row["slug"].as<std::string>();
        tag->name  = row["name"].as<std::string>();
        tag->createTime  = row["create_time"].as<std::string>();
        tag->modifyTime  = row["modify_time"].as<std::string>();
    }
    return tag;
}

TagPtr TagMapper::selectBySlug(const std::string &slug) {
    auto db = app().getDbClient();
    auto result = db->execSqlSync(
        "SELECT id,slug,name,create_time,modify_time FROM tag WHERE slug = ?", slug);
    TagPtr tag;
    if (!result.empty()) {
        auto row = result[0];
        tag = std::make_shared<Tag>();
        tag->id = row["id"].as<int>();
        tag->slug  = row["slug"].as<std::string>();
        tag->name  = row["name"].as<std::string>();
        tag->createTime  = row["create_time"].as<std::string>();
        tag->modifyTime  = row["modify_time"].as<std::string>();
    }
    return tag;
}

TagPtr TagMapper::selectByName(const std::string &name) {
    auto db = app().getDbClient();
    auto result = db->execSqlSync(
        "SELECT id,slug,name,create_time,modify_time FROM tag WHERE name = ?", name);
    TagPtr tag;
    if (!result.empty()) {
        auto row = result[0];
        tag = std::make_shared<Tag>();
        tag->id = row["id"].as<int>();
        tag->slug  = row["slug"].as<std::string>();
        tag->name  = row["name"].as<std::string>();
        tag->createTime  = row["create_time"].as<std::string>();
        tag->modifyTime  = row["modify_time"].as<std::string>();
    }
    return tag;
}

void TagMapper::insert(Tag &tag) {
    auto db = app().getDbClient();
    auto res = db->execSqlSync("INSERT INTO tag (slug,name) VALUE (?,?)",
        tag.slug, tag.name);
    tag.id = res.insertId();
}

void TagMapper::update(Tag &tag) {
    auto db = app().getDbClient();
    db->execSqlSync("UPDATE tag SET slug = ?, name = ? WHERE id = ?",
        tag.slug, tag.name, tag.id);
}

void TagMapper::deletes(std::vector<int> ids) {
    if (ids.empty()) return;
    auto db = app().getDbClient();
    db->execSqlSync("DELETE FROM tag WHERE id IN ?",
        join(ids, "(", ")", ","));
}