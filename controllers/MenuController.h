#pragma once

#include <drogon/drogon.h>
#include "../models/User.h"
#include "../models/Role.h"
#include "../models/Menu.h"
#include "../utils/Utils.h"
#include "../dtos/ApiResponse.h"

using namespace drogon;
using orm::Mapper;
using orm::Criteria;
using orm::CompareOperator;
using orm::SortOrder;
using drogon_model::libre_blog::User;
using drogon_model::libre_blog::Role;
using drogon_model::libre_blog::Menu;

class MenuController : public HttpController<MenuController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(MenuController::getMenuAdmin, "/admin/menu", HttpMethod::Get, "LoginFilter");
    ADD_METHOD_TO(MenuController::getMenu, "/menu", HttpMethod::Get);
    METHOD_LIST_END

    void getMenuAdmin(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

    void getMenu(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;

private:
    static Json::Value generateMenu(Mapper<Menu> &mp, int32_t id);
};