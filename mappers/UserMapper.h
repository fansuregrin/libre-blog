#pragma once

#include <vector>
#include <drogon/drogon.h>
#include "../dtos/User.h"
#include "../utils/Utils.h"

using namespace drogon;

class UserMapper {
public:
    static UserPtr selectByUsername(const std::string &username);

    static UserPtr select(int id);

    static int countByUsername(const std::string &username);

    static int selectRoleId(int id);

    static void insert(User &user);

    static int count();

    static std::vector<User> selectLimit(int page, int pageSize);

    static void update(User &user);

    static void deletes(const std::vector<int> ids);
};