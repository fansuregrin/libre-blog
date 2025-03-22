#include "RoleMapper.h"

std::vector<Role> RoleMapper::selectAll() {
    std::vector<Role> roles;
    auto db = app().getDbClient();
    auto res = db->execSqlSync("SELECT id,name,menu FROM role");
    for (const auto &row : res) {
        Role role;
        role.id = row["id"].as<int>();
        role.name = row["name"].as<std::string>();
        role.menu = row["menu"].as<int>();
        roles.emplace_back(role);
    }
    return roles;
}