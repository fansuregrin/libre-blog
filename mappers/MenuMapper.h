#pragma once

#include <vector>
#include <drogon/drogon.h>
#include "../dtos/Menu.h"
#include "../utils/Utils.h"

using namespace drogon;

class MenuMapper {
public:
    static std::vector<Menu> selectByUser(int id) {
        std::vector<Menu> menus;
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "SELECT id,label,name,parent,ancestor FROM menu "
            "WHERE FIND_IN_SET( "
            "(SELECT m.id FROM user u JOIN role r ON u.role = r.id "
            "JOIN menu m on r.menu = m.id WHERE u.id = ?), ancestor)",
            id
        );
        for (const auto &row : res) {
            Menu m;
            m.id = row["id"].as<int>();
            m.label = row["label"].as<std::string>();
            m.name = row["name"].as<std::string>();
            m.parent = row["parent"].as<int>();
            std::string ancestor = row["ancestor"].as<std::string>();
            m.ancestor = split<int>(ancestor, ",");
            menus.emplace_back(m);
        }
        return menus;
    }
};