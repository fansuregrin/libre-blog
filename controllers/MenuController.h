#pragma once

#include <drogon/drogon.h>
#include "../utils/Utils.h"
#include "../dtos/ApiResponse.h"
#include "../mappers/MenuMapper.h"

using namespace drogon;

class MenuController : public HttpController<MenuController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(MenuController::getMenuAdmin, "/admin/menu", HttpMethod::Get, "LoginFilter");
    METHOD_LIST_END

    void getMenuAdmin(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;
};