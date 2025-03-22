#pragma once

#include <vector>
#include <drogon/drogon.h>
#include "../dtos/Role.h"

using namespace drogon;

class RoleMapper {
public:
    static std::vector<Role> selectAll();
};