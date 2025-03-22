#pragma once

#include <vector>
#include <drogon/drogon.h>
#include "../dtos/Menu.h"
#include "../utils/Utils.h"

using namespace drogon;

class MenuMapper {
public:
    static std::vector<Menu> selectByUser(int id);
};