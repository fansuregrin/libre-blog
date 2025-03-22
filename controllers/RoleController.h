#pragma once

#include <drogon/drogon.h>
#include "TransformRequest.h"
#include "../dtos/ApiResponse.h"
#include "../mappers/UserMapper.h"
#include "../mappers/RoleMapper.h"
#include "../exceptions/PermissionException.h"

using namespace drogon;

class RoleController : public HttpController<RoleController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(RoleController::roleList, "/admin/roles", HttpMethod::Get, "LoginFilter");
    METHOD_LIST_END

    void roleList(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;
};