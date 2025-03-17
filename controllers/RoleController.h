#pragma once

#include <drogon/drogon.h>
#include "TransformRequest.h"
#include "../models/Role.h"
#include "../models/User.h"
#include "../dtos/ApiResponse.h"
#include "../exceptions/PermissionException.h"

using namespace drogon;
using orm::Mapper;
using orm::Criteria;
using drogon_model::libre_blog::Role;
using drogon_model::libre_blog::User;

class RoleController : public HttpController<RoleController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(RoleController::roleList, "/roles", HttpMethod::Get, "LoginFilter");
    METHOD_LIST_END

    void roleList(
        const HttpRequestPtr& req,
        std::function<void (const HttpResponsePtr &)> &&callback
    ) const;
};