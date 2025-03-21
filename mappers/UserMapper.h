#pragma once

#include <vector>
#include <drogon/drogon.h>
#include "../dtos/User.h"

using namespace drogon;

class UserMapper {
public:
    static UserPtr selectByUsername(const std::string &username) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "SELECT id,username,realname,email,password,create_time,modify_time,role "
            "FROM user WHERE username = ?",
            username);
        UserPtr user;
        if (!res.empty()) {
            auto row = res[0];
            user = std::make_shared<User>();
            user->id = row["id"].as<int>();
            user->username = row["username"].as<std::string>();
            user->password = row["password"].as<std::string>();
            user->createTime = row["create_time"].as<std::string>();
            user->modifyTime = row["modify_time"].as<std::string>();
            user->email = row["email"].as<std::string>();
            user->realname = row["realname"].as<std::string>();
            user->role = row["role"].as<int>();
        }
        return user;
    }

    static UserPtr select(int id) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "SELECT id,username,realname,email,password,create_time,modify_time,role "
            "FROM user WHERE id = ?",
            id);
        UserPtr user;
        if (!res.empty()) {
            auto row = res[0];
            user = std::make_shared<User>();
            user->id = row["id"].as<int>();
            user->username = row["username"].as<std::string>();
            user->password = row["password"].as<std::string>();
            user->createTime = row["create_time"].as<std::string>();
            user->modifyTime = row["modify_time"].as<std::string>();
            user->email = row["email"].as<std::string>();
            user->realname = row["realname"].as<std::string>();
            user->role = row["role"].as<int>();
        }
        return user;
    }

    static int countByUsername(const std::string &username) {
        if (username.empty()) return 0;
        auto db = app().getDbClient();
        auto res = db->execSqlSync("SELECT COUNT(*) FROM user WHERE username = ?",
            username);
        if (!res.empty()) {
            return res[0][0].as<int>();
        } else {
            return 0;
        }
    }

    static int selectRoleId(int id) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync("SELECT role FROM user WHERE id = ?", id);
        if (res.empty()) {
           return -1; 
        } else {
            return res[0][0].as<int>();
        }
    }

    static void insert(User &user) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "INSERT INTO user (username,password,email,realname,create_time,modify_time,role) "
            "VALUES (?,?,?,?,?,NOW(),NOW(),?)",
            user.username, user.password, user.email, user.realname, user.role
        );
        user.id = res.insertId();
    }

    static int count() {
        auto db = app().getDbClient();
        return db->execSqlSync("SELECT COUNT(*) FROM user")[0][0].as<int>();
    }

    static std::vector<User> selectLimit(int page, int pageSize) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync(
            "SELECT id,username,realname,email,role,create_time,modify_time "
            "ROM user LIMIT ?,? ORDER BY modify_time DESC",
            (page-1) * pageSize, pageSize
        );
        std::vector<User> users;
        for (const auto &row : res) {
            User user;
            user.id = row["id"].as<int>();
            user.username = row["username"].as<std::string>();
            user.realname = row["realname"].as<std::string>();
            user.email = row["email"].as<std::string>();
            user.createTime = row["create_time"].as<std::string>();
            user.role = row["role"].as<int>();
            users.emplace_back(user);
        }
        return users;
    }

    static void update(User &user) {
        auto db = app().getDbClient();
        auto res = db->execSqlSync("UPDATE user SET modify_time = NOW() "
            + (user.username.empty() ? "" : ",username = " + user.username)
            + (user.password.empty() ? "" : ",password = " + user.password)
            + (user.email.empty() ? "" : ",email = " + user.email)
            + (user.realname.empty() ? "" : ",realname = " + user.realname)
            + (user.role > 0 ? ",role = " + std::to_string(user.role) : "")
            + " WHERE id = ?",
            user.id
        );
    }

    static void deletes(const std::vector<int> ids) {
        if (ids.empty()) return;
        auto db = app().getDbClient();
        db->execSqlSync("DELETE FROM user WHERE id IN ?", join(ids, "(", ")", ","));
    }

};